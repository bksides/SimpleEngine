
#include "SinglePlayerGame.h"
#include "RaceApplication.h"
#include "NetworkServer.h"
#include "NetworkProtocol.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <OgreMeshManager.h>
#include <OgreQuaternion.h>
#include <OgreMath.h>
#include <OISKeyboard.h>
#include <utility>

const char ack[4] = "ack";

//-------------------------------------------------------------------------------------
RaceApplication::RaceApplication(void)
{
    //game = new SinglePlayerGame(mCamera, mSceneMgr, mShutDown, pause_pop_up);
}
//-------------------------------------------------------------------------------------
RaceApplication::~RaceApplication(void)
{
    delete game;
}

CEGUI::Key InjectOISKey(OIS::KeyEvent inKey, bool bButtonDown)
{
    if (bButtonDown)
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)inKey.key);
        CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(inKey.text);
    }
    else
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)inKey.key);
    }
}

void clientLobbyMode(RaceApplication* app)
{
    IPaddress ip;
    SDLNet_ResolveHost(&ip, app->toConnect->getText().c_str(), 2800);
    TCPsocket sock = SDLNet_TCP_Open(&ip);
    int i;
    char playername[100];
    while(true)
    {
        char messageType[11];
        if(SDLNet_TCP_Recv(sock, (void*)&messageType, 11) <= 0)
        {
            /*
                Eventually kick the client back to the main menu
                upon failed connection.
            */
            break;
        }
        if(!strcmp(messageType, "start"))
        {
            std::cout << "STARTING GAME\n";
        }
        else if(!strcmp(messageType, "playername"))
        {
            if(SDLNet_TCP_Recv(sock, (void*)&i, 1) <= 0)
            {
                /*
                    Eventually kick the client back to the main menu
                    upon failed connection.
                */
                break;
            }
            std::cout << i << ": ";
            if(SDLNet_TCP_Recv(sock, (void*)&playername, 100) <= 0)
            {
                break;
            }
            std::cout << playername << "\n";
            app->player_slots[i]->setText(std::string(playername));
            if(SDLNet_TCP_Send(sock, (void*)&ack, 3) < 3)
            {
                break;
            }
        }
    }
}

void RaceApplication::CEGUI_Init()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(true);

/*    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);*/

/*  Use this if you want a cool demo of what CEGUI can do with sheets
    CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TextDemo.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);
*/
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    pause_pop_up = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/PausePopUp");
    
    createStartMenu(wmgr);
    createMultiPlayerMenu(wmgr);
    createJoinMenu(wmgr);
    
    //score_board->setProperty("HorzFormatting","HorzCentred");
    pause_pop_up->setText("You Win!");
    pause_pop_up->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(.25, 0)));
    pause_pop_up->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.5, 0)));
    pause_pop_up->setVisible(false);

    sheet->addChild(pause_pop_up);
    sheet->addChild(start_menu);
    sheet->addChild(mult_menu);
    sheet->addChild(join_menu);
    
    //mult_menu->setSize(CEGUI::USize(CEGUI::UDim(1.0,0.0), CEGUI::UDim(1.0, 0.0)));
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

bool RaceApplication::keyPressed( const OIS::KeyEvent &arg )
{
    InjectOISKey(arg, true);
    if(game != NULL)
    {
        return game->keyPressed(arg);
    }
}

bool RaceApplication::keyReleased( const OIS::KeyEvent &arg)
{
    InjectOISKey(arg, false);
    if(game != NULL)
    {
        return game->keyReleased(arg);
    }
}

void RaceApplication::createMultiPlayerMenu(CEGUI::WindowManager& wmgr)
{
    mult_menu = wmgr.createWindow("TaharezLook/StaticImage", "CEGUIDemo/MPBackground");

    //CEGUI::Imageset* MenuImageset = CEGUI::ImagesetManager::getSingleton().createImagesetFromImageFile("Background","MenuBackground.jpg");
    //MenuImageset->defineImage("Background", CEGUI::Point(0.0f,0.0f), CEGUI::Size( 1.0f, 1.0f ), Point(0.0f,0.0f));
    
    //start_menu->setProperty("Image", "menuBackground/menuBackground");

    mult_menu->setProperty("Image", "TaharezLook/ClientBrush");
    
    mult_menu->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0,0)));
    mult_menu->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));



    CEGUI::Window* menu = wmgr.createWindow("TaharezLook/StaticImage", "CEGUIDemo/MPMenu");
    mult_menu->addChild(menu);
    menu->setProperty("Image","OgreTrayImages/BandsFull");
    menu->setRiseOnClickEnabled(false);

    menu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0), CEGUI::UDim(0.1,0)));
    menu->setSize(CEGUI::USize(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.8, 0)));

    mult_info = wmgr.createWindow("TaharezLook/StaticText","CEGUIDemo/MPMenuTitle");
    mult_menu->addChild(mult_info);
    mult_info->setText("Select if you're hosting or joining a game.\n\nIf you're joining, enter the address of the host you want to connect to.");
    mult_info->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0.0), CEGUI::UDim(0.15, 0.0)));
    mult_info->setSize(CEGUI::USize(CEGUI::UDim(0.7,0.0), CEGUI::UDim(0.15, 0.0)));

    CEGUI::FrameWindow* window = (CEGUI::FrameWindow*)wmgr.createWindow("TaharezLook/FrameWindow", "CEGUIDemo/MPWindow");
    mult_menu->addChild(window);
    window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2,0), CEGUI::UDim(0.35,0)));
    window->setSize(CEGUI::USize(CEGUI::UDim(0.6,0.0), CEGUI::UDim(0.5, 0.0)));
    window->setRiseOnClickEnabled(false);
    window->setRollupEnabled(false);
    window->setDragMovingEnabled(false);
    window->setSizingEnabled(false);

    hostOption = (CEGUI::RadioButton*)wmgr.createWindow("TaharezLook/RadioButton", "CEGUIDemo/MPHostOption");
    window->addChild(hostOption);
    hostOption->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0), CEGUI::UDim(0.2,0)));
    hostOption->setSize(CEGUI::USize(CEGUI::UDim(0.1,0.0), CEGUI::UDim(0.1, 0.0)));
    hostOption->setText("Host");
    hostOption->setGroupID(1);
    hostOption->setSelected(true);
    hostOption->setID(0);
//  hostOption->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&PongApplication::testRadios(hostOption), this));

    CEGUI::RadioButton* clientOption = (CEGUI::RadioButton*)wmgr.createWindow("TaharezLook/RadioButton", "CEGUIDemo/MPClientOption");
    window->addChild(clientOption);
    clientOption->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3,0), CEGUI::UDim(0.2,0)));
    clientOption->setSize(CEGUI::USize(CEGUI::UDim(0.1,0.0), CEGUI::UDim(0.1, 0.0)));
    clientOption->setText("Client");
    clientOption->setGroupID(1);
    clientOption->setID(1);
    //hostOption->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&PongApplication::toggleIP(), this));

    toConnect = (CEGUI::Editbox*)wmgr.createWindow("TaharezLook/Editbox", "CEGUIDemo/MPhostname");
    window->addChild(toConnect);
    toConnect->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0), CEGUI::UDim(0.4,0)));
    toConnect->setSize(CEGUI::USize(CEGUI::UDim(0.8,0.0), CEGUI::UDim(0.2, 0.0)));
    toConnect->setReadOnly(false);
    toConnect->setTextMasked(false);

    CEGUI::PushButton* startGame = (CEGUI::PushButton*)wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/MPStartGame");
    window->addChild(startGame);
    startGame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0), CEGUI::UDim(0.7,0)));
    startGame->setSize(CEGUI::USize(CEGUI::UDim(0.8,0.0), CEGUI::UDim(0.2, 0.0)));
    startGame->setText("Start multiplayer game");
    startGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&RaceApplication::showJoinMenu, this));
    //add editable text field -- should only be able to edit if clientOption is selected
    //add button to go back to main menu

    CEGUI::PushButton* backToStartMenu = (CEGUI::PushButton*)wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/MPBackToStart");
    window->addChild(backToStartMenu);
    backToStartMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05,0), CEGUI::UDim(0.02,0)));
    backToStartMenu->setSize(CEGUI::USize(CEGUI::UDim(0.2,0.0), CEGUI::UDim(0.15, 0.0)));
    backToStartMenu->setText("Back");
    backToStartMenu->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&RaceApplication::backToMenu, this));


    mult_menu->setVisible(false);
}

void RaceApplication::showMultiPlayerOptions(void)
{
    start_menu->setVisible(false);
    mult_menu->setVisible(true);
}

void RaceApplication::backToMenu(void)
{
    mult_menu->setVisible(false);
    mult_info->setText("Select if you're hosting or joining a game.\n\nIf you're joining, enter the address of the host you want to connect to.");
    start_menu->setVisible(true);
}

//-------------------------------------------------------------------------------------
void RaceApplication::createScene(void)
{
    start_menu->setVisible(false);
    mult_menu->setVisible(false);
    //score_board->setVisible(true);
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

    game = new SinglePlayerGame(mCamera, mSceneMgr, mShutDown, pause_pop_up);

    if(game != NULL)
    {
        createCamera();
        createViewports();
        game->createScene();
    }

    startgame = true;
}

void RaceApplication::createStartMenu(CEGUI::WindowManager& wmgr)
{
    //start_menu = wmgr.createWindow("TaharezLook/FrameWindow","CEGUIDemo/StartMenu");
    
    start_menu = wmgr.createWindow("TaharezLook/StaticImage", "CEGUIDemo/Background");

    //CEGUI::Imageset* MenuImageset = CEGUI::ImagesetManager::getSingleton().createImagesetFromImageFile("Background","MenuBackground.jpg");
    //MenuImageset->defineImage("Background", CEGUI::Point(0.0f,0.0f), CEGUI::Size( 1.0f, 1.0f ), Point(0.0f,0.0f));
    
    //start_menu->setProperty("Image", "menuBackground/menuBackground");

    start_menu->setProperty("Image", "TaharezLook/ClientBrush");
    
    start_menu->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0,0)));
    start_menu->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

    //CEGUI::FrameWindow* menu = (CEGUI::FrameWindow*)wmgr.createWindow("TaharezLook/FrameWindow","CEGUIDemo/Menu");
    CEGUI::Window* menu = wmgr.createWindow("TaharezLook/StaticImage", "CEGUIDemo/Menu");
    start_menu->addChild(menu);
    menu->setProperty("Image","OgreTrayImages/TrayTR");
    menu->setRiseOnClickEnabled(false);
    //menu->setRollupEnabled(false);
    //menu->setDragMovingEnabled(false);
    menu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1,0), CEGUI::UDim(0.1,0)));
    menu->setSize(CEGUI::USize(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.8, 0)));

    CEGUI::Window* title = wmgr.createWindow("TaharezLook/StaticText","CEGUIDemo/menuTitle");
    start_menu->addChild(title);
    title->setText("Welcome to Penguin Racing! \nChoose a mode to start playing.");
    title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0.0), CEGUI::UDim(0.15, 0.0)));
    title->setSize(CEGUI::USize(CEGUI::UDim(0.7,0.0), CEGUI::UDim(0.15, 0.0)));

    //when a mode is selected, the handler should hide the start menu
    //and start the mode they picked
    //also the cursor should be hidden
    CEGUI::PushButton* singPlayer = (CEGUI::PushButton*)wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/SingPlayer");
    start_menu->addChild(singPlayer);
    singPlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0.0), CEGUI::UDim(0.5, 0.0)));
    singPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.4,0.0), CEGUI::UDim(0.15, 0.0)));
    singPlayer->setText("Single Player");
    singPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&RaceApplication::createScene, this));

    CEGUI::PushButton* multiPlayer = (CEGUI::PushButton*)wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/MultiPlayer");
    start_menu->addChild(multiPlayer);
    multiPlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0.0), CEGUI::UDim(0.7, 0.0)));
    multiPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.4,0.0), CEGUI::UDim(0.15, 0.0)));
    multiPlayer->setText("Multiplayer");
    //multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&RaceApplication::createScene, this));
    multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&RaceApplication::showMultiPlayerOptions, this));
}

void RaceApplication::createJoinMenu(CEGUI::WindowManager& wmgr)
{
    join_menu = wmgr.createWindow("TaharezLook/StaticImage", "CEGUIDemo/Background");
    join_menu->setProperty("Image", "TaharezLook/ClientBrush");
    
    join_menu->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0), CEGUI::UDim(0,0)));
    join_menu->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

    CEGUI::Window* title = wmgr.createWindow("TaharezLook/StaticText","CEGUIDemo/menuTitle");
    join_menu->addChild(title);
    title->setText("Multiplayer Lobby");
    title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0.0), CEGUI::UDim(0.15, 0.0)));
    title->setSize(CEGUI::USize(CEGUI::UDim(0.7,0.0), CEGUI::UDim(0.15, 0.0)));

    for(int i = 0; i < 16; ++i)
    {
        player_slots[i] = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/ScoreBoard");
        player_slots[i]->setProperty("HorzFormatting","HorzCentred");
        player_slots[i]->setText("--");
        player_slots[i]->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2 * (i / 4) + 0.125, 0), CEGUI::UDim(0.1 * (i%4) + .4, 0)));
        player_slots[i]->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

        join_menu->addChild(player_slots[i]);
    }

    join_menu->setVisible(false);
}

void RaceApplication::showJoinMenu(void)
{
    bool client = (hostOption->getSelectedButtonInGroup()->getID()==1) ? true : false;
    //printf("Here's what was typed: %s\n", toConnect->getText().c_str());
    char* typedText = const_cast<char *>(toConnect->getText().c_str());
    //printf("The selected type was %s.\n", client?"client":"host");
    char* hostname = NULL;
    if(client)
        hostname = typedText;
    else
        hostname = NULL;
    //error checking
    if(client && strcmp(typedText, "")==0)
    {
        mult_info->setText("ERROR: You must enter the hostname of who you're connecting to if you're not hosting!!");
        //Mix_PlayChannel(-1, error, 0);
        return;
    }
    if(!client && strcmp(typedText, "")!=0)
    {
        mult_info->setText("ERROR: If you're hosting, you shouldn't enter anything in the textbox.");
        //Mix_PlayChannel(-1, error, 0);
        return;       
    }
    mult_info->setText("PLEASE WAIT\n\nWaiting for someone to connect...");
    mult_menu->setVisible(false);

    join_menu->setVisible(true);

    if(client)
    {
        std::thread* newthread = new std::thread(clientLobbyMode, this);
    }
    else
    {
        serverLobbyMode();
    }
}

void RaceApplication::serverLobbyMode()
{
    NetworkProtocol* protocol = new NetworkProtocol();
    NetworkServer* server = new NetworkServer(2800, protocol);
    std::function<std::pair<char*, int>(int*)> playerName = [this](int* n) -> std::pair<char*, int> {
        if(n != NULL)
        {
            return std::pair<char*, int>(const_cast<char*>(this->player_slots[*n]->getText().c_str()), this->player_slots[*n]->getText().length());
        }
        else
        {
            return std::pair<char*, int>("--", 2);
        }
    };
    protocol->addFunction<char, int>(0, playerName);
    server->accept = [this](TCPsocket sock) {
        static int num = 1;
        this->socket_to_player_slot[sock] = num;
        IPaddress* addr = SDLNet_TCP_GetPeerAddress(sock);
        const char* addrstr = SDLNet_ResolveIP(addr);
        //std::string addrstdstr(addrstr);
        std::cout << addrstr << "\n\n\n";
        //std::cout << addrstdstr << "\n\n\n";
        this->player_slots[num]->setText(addrstr);
        if(num == 2)
        {
            this->startgame = true;
        }
        ++num;
        num %= 16;
    };

    server->handle = [this](TCPsocket sock, NetworkServer* server) {
        char clientack[4];
        char* messageTypes[2] = {"playername", "start"};
        if(this->startgame)
        {
            if(SDLNet_TCP_Send(sock, messageTypes[1], 6) < 6)
            {
                //SDLNet_TCP_Close(sock);
                server->terminateClientSock(sock);
                this->player_slots[this->socket_to_player_slot[sock]]->setText("--");
                return;
            }
            //this->startgame = false;
        }
        else
        {
            for(int i = 0; i < 16; ++i)
            {
                if(SDLNet_TCP_Send(sock, messageTypes[0], 11) < 11)
                {
                    //SDLNet_TCP_Close(sock);
                    server->terminateClientSock(sock);
                    this->player_slots[this->socket_to_player_slot[sock]]->setText("--");
                    return;
                }
                if(SDLNet_TCP_Send(sock, &i, 1) < 1)
                {
                    //SDLNet_TCP_Close(sock);
                    server->terminateClientSock(sock);
                    this->player_slots[this->socket_to_player_slot[sock]]->setText("--");
                    return;
                }
                if(SDLNet_TCP_Send(sock, (void*)(this->player_slots[i])->getText().c_str(), this->player_slots[i]->getText().length() + 1) < this->player_slots[i]->getText().length() + 1)
                {
                    //SDLNet_TCP_Close(sock);
                    server->terminateClientSock(sock);
                    this->player_slots[this->socket_to_player_slot[sock]]->setText("--");
                    return;
                }
                if(SDLNet_TCP_Recv(sock, &clientack, 4) <= 0)
                {
                    //SDLNet_TCP_Close(sock);
                    server->terminateClientSock(sock);
                    this->player_slots[this->socket_to_player_slot[sock]]->setText("--");
                    return;
                }
            }
        }
    };

    server->go();
}

//--------------------------------------------------------------------------------------
bool RaceApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(game != NULL)
    {
        game->frameRenderingQueued(evt);
    }
    return BaseApplication::frameRenderingQueued(evt);
}

//--------------------------------------------------------------------------------------
void RaceApplication::createCamera()
{
    if(game != NULL)
    {
        game->createCamera();
    }
}

//--------------------------------------------------------------------------------------
void RaceApplication::createViewports()
{
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0.3));
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) /
        Ogre::Real(vp->getActualHeight()));
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        if(argc > 1)
        {
            std::cout << "\n\n\nTRYING TO CONNECT\n\n\n";
            IPaddress ip;
            SDLNet_ResolveHost(&ip, "cancer.cs.utexas.edu", 2800);
            TCPsocket sock = SDLNet_TCP_Open(&ip);
            if(!sock)
            {
                std::cout << "\n\n\nCONNECTION ERROR\n\n\n";
            }
        }

        // Create application object
        RaceApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
