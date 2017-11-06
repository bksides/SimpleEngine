#include "PongApplication.h"
#include "Paddle.h"
#include "NetPaddle.h"
#include "Wall.h"
#include "PongBall.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <OgreMath.h>
#include <btBulletDynamicsCommon.h>
#include <OISKeyboard.h>
#include <cstdint>
#include <string.h>
#include <stdio.h>
#include <cstring>

using namespace SimpleEngine;

Mix_Chunk* boing = NULL;
Mix_Music* music = NULL;
Mix_Chunk* ching = NULL;
Mix_Chunk* lose = NULL;
Mix_Chunk* error = NULL;

GameObject* paddle;
GameObject* netPaddle;
GameObject* ball;
GameObject* wall = NULL;
GameObject* frontWall = NULL;
GameObject* backWall = NULL;
Ogre::Vector3 ballMostRecentSentPosition = Ogre::Vector3::ZERO;
int vel = 30;
int player_score = 0;
int opponent_score =0;
bool client = false;
PongApplication app;
CEGUI::Window *score_board;
CEGUI::Window *pause_pop_up;
CEGUI::Window *start_menu;
CEGUI::Window *mult_menu;
CEGUI::Window* mult_info;
CEGUI::RadioButton* hostOption;
CEGUI::Editbox* toConnect;
bool gameOver = false;
bool CEGUI_needs_init = true;
bool multiplayer = false;
char* hostname = NULL;
std::thread* netthread = NULL;
bool terminating = false;

bool playBoing(btManifoldPoint& cp, void* body0, void* body1)
{
    if(sound)
    {
        Mix_PlayChannel( -1, boing, 0 );
    }
    if(!multiplayer && ((body0 == ball->getRigidBody() && body1 == wall->getRigidBody())||
        (body1 == ball->getRigidBody() && body0 == wall->getRigidBody())))
    {
        ++player_score;
        //quit->setText(std::to_string(player_score));
        app.updateScoreboard();
        vel += 3;
        if(sound)
        {
            Mix_PlayChannel(-1, ching, 0);
        }
    }
    if(multiplayer)
    {
        if((body0 == ball->getRigidBody() && body1 == netPaddle->getRigidBody()) ||
        (body1 == ball->getRigidBody() && body0 == netPaddle->getRigidBody()))
        {
            //quit->setText(std::to_string(player_score));
            vel += 3;
        }
    }   
    if(body0 == ball->getRigidBody() && body1 == frontWall->getRigidBody() ||
        body1 == ball->getRigidBody() && body0 == frontWall->getRigidBody())
    {
        if(!gameOver)
        {
            Mix_PlayChannel(-1, lose, 0);
        }
        if(!multiplayer)
        {
            score_board->setText("You Lose!");
            gameOver = true;
        }
        else
        {
            if(!client)
            {
                ++opponent_score;
                app.updateScoreboard();
            }
            ball->setPosition(Ogre::Vector3::ZERO);
            ball->setVelocity(Ogre::Vector3(Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40)));
            vel = 30;
        }
    }
    if(multiplayer && (body0 == ball->getRigidBody() && body1 == backWall->getRigidBody() ||
        body1 == ball->getRigidBody() && body0 == backWall->getRigidBody()))
    {
        ball->setPosition(Ogre::Vector3::ZERO);
        ball->setVelocity(Ogre::Vector3(Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40)));
        vel = 30;
        if(sound)
        {
            Mix_PlayChannel(-1, ching, 1);
        }
        if(!client)
        {
            player_score++;
        }
        app.updateScoreboard();
    }
    return true;
}

//-------------------------------------------------------------------------------------
PongApplication::PongApplication(void)
{
	srand((unsigned)time(NULL));

    if( SDL_Init( SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        mShutDown = true;
    }
    if(Mix_Init(MIX_INIT_MP3))
    {
        printf("Sound mixer could not initialize!");
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        mShutDown = true;
    }
    boing = Mix_LoadWAV( "./dist/media/sounds/boing.wav" );
    music = Mix_LoadMUS("./dist/media/sounds/music.mp3");
    ching = Mix_LoadWAV("./dist/media/sounds/score.wav");
    lose = Mix_LoadWAV("./dist/media/sounds/gameover.wav");
    error = Mix_LoadWAV("./dist/media/sounds/error.wav");
    //Mix_VolumeChunk(ching, MIX_MAX_VOLUME);

    if( boing == NULL || music == NULL || ching == NULL || lose == NULL)
    {
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        mShutDown = true;
    }

    //player_score = 0;
}
//-------------------------------------------------------------------------------------
PongApplication::~PongApplication(void)
{
	//Destructor; time to delete pointers, free SDL structures, etc.
}

//-------------------------------------------------------------------------------------
/*
bool playBoing(btManifoldPoint& cp, void* body0, void* body1)
{
    Mix_PlayChannel( -1, boing, 0 );
    if((body0 == ball->getRigidBody() && body1 == paddle->getRigidBody())||
        (body1 == ball->getRigidBody() && body0 == paddle->getRigidBody()))
    {
        ++PongApplication::player_score;

    }
}
*/

void PongApplication::updateScoreboard(void)
{
    if(multiplayer)
    {
        score_board->setText("You: "+std::to_string(player_score)+ " | Opp: "+std::to_string(opponent_score));
    }
    else
    {
        score_board->setText("Score: "+std::to_string(player_score));
    }
    if(multiplayer && (opponent_score >= 10 || player_score >= 10))
    {
        gameOverScreen(player_score >= 10);
    }
}

void PongApplication::createScene(void)
{
    if(CEGUI_needs_init)
    {
        CEGUI_Init();
        CEGUI_needs_init = false;
    }
	//Here we should initialize the PongWorld and populate it with GameObjects
/*    Ogre::Light* lamp = mSceneMgr->createLight("lamp");
    lamp->setType(Ogre::Light::LT_POINT);
    lamp->setPosition(0,49,-70);
    lamp->setDiffuseColour(1,1,1);
    lamp->setSpecularColour(1,1,1);
    lamp->setAttenuation(200, 0, 0, .0002);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    //Create wall entities
    wallWorld = new World(mSceneMgr);

    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO);
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / 2));
    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / -2));
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI));
	wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));

    ball = new PongBall(mSceneMgr, btVector3(0,0,0));

    if(client)
    {
        ball->getRigidBody()->setCollisionFlags(ball->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    }

    wallWorld->addObject(ball, Ogre::Vector3::ZERO, Ogre::Vector3(Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40)));

    paddle = new Paddle(mSceneMgr);
    wallWorld->addObject(paddle, Ogre::Vector3(0, 0, -49), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));

    gContactProcessedCallback = playBoing;

    Mix_PlayMusic(music, -1);*/
}

void PongApplication::createMultiPlayerScene(TCPsocket socket)
{
    player_score = 0;
    opponent_score = 0;

    start_menu->setVisible(false);
    score_board->setText("You: " + std::to_string(player_score) + " | Opp: " + std::to_string(opponent_score));
    score_board->setVisible(true);
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

    Ogre::Light* lamp = mSceneMgr->createLight("lamp");
    lamp->setType(Ogre::Light::LT_DIRECTIONAL);
    lamp->setDirection(0,-1,0);
    lamp->setDiffuseColour(1,1,1);
    lamp->setSpecularColour(1,1,1);
    lamp->setAttenuation(200, 0, 0, .0002);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    //Create wall entities
    wallWorld = new World(mSceneMgr);

    backWall = new Wall(mSceneMgr);
    frontWall = new Wall(mSceneMgr);

    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO);
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / 2));
    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / -2));
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI));
    wallWorld->addObject(frontWall, -60*Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / 2, 0, 0));
    wallWorld->addObject(backWall, 60*Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / 2, 0, 0));

    ball = new PongBall(mSceneMgr, btVector3(0,0,0));

    wallWorld->addObject(ball, Ogre::Vector3::ZERO, Ogre::Vector3(Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40)));

    paddle = new Paddle(mSceneMgr);
    netPaddle = new NetPaddle(mSceneMgr, socket);
    wallWorld->addObject(paddle, Ogre::Vector3(0, 0, -49), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));
    wallWorld->addObject(netPaddle, Ogre::Vector3(0, 0, 49), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));

    gContactProcessedCallback = playBoing;

    if(CEGUI_needs_init)
    {
        CEGUI_Init();
        CEGUI_needs_init = false;
    }

    Mix_PlayMusic(music, -1);
}

void PongApplication::beginGame(void)
{
    bool ready = true;
    SDLNet_TCP_Send(app.sock, &ready, sizeof(bool));
    SDLNet_TCP_Recv(app.sock, &ready, sizeof(bool));
    mRoot->startRendering();
}

bool PongApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if(arg.key == OIS::KC_P)
    {
        if(!multiplayer)
        {
            if(!gameOver)
                wallWorld->pause(!wallWorld->isPaused());
            if(wallWorld->isPaused() && !gameOver)
            {
                //score_board->setText("Paused");
                pause_pop_up->setVisible(true);
            }
            else
            {
                score_board->setText("Score: "+std::to_string(player_score));
                pause_pop_up->setVisible(false);
            }
        }
    }
    if(!multiplayer && arg.key == OIS::KC_RETURN && wallWorld->isPaused())
    {
        vel = 30;
        player_score = 0;
        score_board->setText("Score: "+std::to_string(player_score));
        gameOver = false;
        pause_pop_up->setVisible(false);
        pause_pop_up->setText("Game Paused.\n\nControls:\n\nEnter: Start over(pause/game over only)\nArrow keys: Move the ball\nM: Mute the music\nS: Mute the sound effects\nPage Up/Page Down: Control music volume\nESC: Exit the game\nStop, Drop, Roll: Put out the fire");
        if(netthread != NULL)
        {
            terminating = true;
            netthread->join();
            terminating = false;
        }
        mSceneMgr->clearScene();
        if(!multiplayer)
        {
            beginSinglePlayer();
        }
    }
    if(arg.key == OIS::KC_PGUP)
    {
        Mix_VolumeMusic(Mix_VolumeMusic(-1) + 20);
    }
    if(arg.key == OIS::KC_PGDOWN)
    {
        Mix_VolumeMusic(Mix_VolumeMusic(-1) - 20);
    }
    if(arg.key == OIS::KC_M)
    {
        if(Mix_PausedMusic())
        {
            Mix_ResumeMusic();
        }
        else
        {
            Mix_PauseMusic();
        }
    }
    return BaseApplication::keyPressed(arg);
}

void PongApplication::CEGUI_Init(void)
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

/*    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);*/

/*  Use this if you want a cool demo of what CEGUI can do with sheets
    CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TextDemo.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);
*/
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    createStartMenu(wmgr);
    createScoreBoard(wmgr);
    createPauseMenu(wmgr);
    createMultiPlayerMenu(wmgr);

    /* remove this to show the menu */
    //start_menu->setVisible(false);

    sheet->addChild(score_board);
    sheet->addChild(pause_pop_up);
    sheet->addChild(start_menu);
    sheet->addChild(mult_menu);
    mult_menu->setSize(CEGUI::USize(CEGUI::UDim(1.0,0.0), CEGUI::UDim(1.0, 0.0)));
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

void PongApplication::createStartMenu(CEGUI::WindowManager& wmgr)
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
    title->setText("Welcome to Ogre Ball! \nChoose a mode to start playing.");
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
    singPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PongApplication::beginSinglePlayer, this));

    CEGUI::PushButton* multiPlayer = (CEGUI::PushButton*)wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/MultiPlayer");
    start_menu->addChild(multiPlayer);
    multiPlayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0.0), CEGUI::UDim(0.7, 0.0)));
    multiPlayer->setSize(CEGUI::USize(CEGUI::UDim(0.4,0.0), CEGUI::UDim(0.15, 0.0)));
    multiPlayer->setText("Multiplayer");
    //multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PongApplication::beginMultiPlayer, this));
    multiPlayer->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PongApplication::showMultiPlayerOptions, this));
}

void PongApplication::createScoreBoard(CEGUI::WindowManager& wmgr)
{
    score_board = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/ScoreBoard");

    score_board->setProperty("HorzFormatting","HorzCentred");
    if(multiplayer)
    {
        score_board->setText("You: "+std::to_string(player_score)+ " | Opp: "+std::to_string(opponent_score));
    }
    else
    {
        score_board->setText("Score: "+std::to_string(player_score));
    }
    score_board->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0, 0)));
    score_board->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    score_board->setVisible(false);
}

void PongApplication::createPauseMenu(CEGUI::WindowManager& wmgr)
{
    pause_pop_up = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/PausePopUp");

    score_board->setProperty("HorzFormatting","HorzCentred");
    pause_pop_up->setText("Game Paused\n\nControls:\n\nEnter: Start over(pause/game over only)\nArrow keys: Move the ball\nM: Mute the music\nS: Mute the sound effects\nPage Up/Page Down: Control music volume\nESC: Exit the game\nStop, Drop, Roll: Put out the fire");
    pause_pop_up->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(.25, 0)));
    pause_pop_up->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.5, 0)));
    pause_pop_up->setVisible(false);
}

void PongApplication::createMultiPlayerMenu(CEGUI::WindowManager& wmgr)
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
    startGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PongApplication::prepareMultiPlayer, this));
    //add editable text field -- should only be able to edit if clientOption is selected
    //add button to go back to main menu

    CEGUI::PushButton* backToStartMenu = (CEGUI::PushButton*)wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/MPBackToStart");
    window->addChild(backToStartMenu);
    backToStartMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05,0), CEGUI::UDim(0.02,0)));
    backToStartMenu->setSize(CEGUI::USize(CEGUI::UDim(0.2,0.0), CEGUI::UDim(0.15, 0.0)));
    backToStartMenu->setText("Back");
    backToStartMenu->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PongApplication::backToMenu, this));


    mult_menu->setVisible(false);
}

void PongApplication::backToMenu(void)
{
    mult_menu->setVisible(false);
    mult_info->setText("Select if you're hosting or joining a game.\n\nIf you're joining, enter the address of the host you want to connect to.");
    start_menu->setVisible(true);
}

void PongApplication::beginSinglePlayer(void)
{
    start_menu->setVisible(false);
    score_board->setVisible(true);
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
    //actuall create the single player scene
    Ogre::Light* lamp = mSceneMgr->createLight("lamp");
    lamp->setType(Ogre::Light::LT_POINT);
    lamp->setPosition(0,49,-70);
    lamp->setDiffuseColour(1,1,1);
    lamp->setSpecularColour(1,1,1);
    lamp->setAttenuation(200, 0, 0, .0002);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    //Create wall entities
    wallWorld = new World(mSceneMgr);

    wall = new Wall(mSceneMgr);
    frontWall = new Wall(mSceneMgr);
    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO);
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / 2));
    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / -2));
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI));
    wallWorld->addObject(wall,  50*Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));
    wallWorld->addObject(frontWall, -60*Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / 2, 0, 0));

    ball = new PongBall(mSceneMgr, btVector3(0,0,0));

    wallWorld->addObject(ball, Ogre::Vector3::ZERO, Ogre::Vector3(Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(-40, 40)));

    paddle = new Paddle(mSceneMgr);
    wallWorld->addObject(paddle, Ogre::Vector3(0, 0, -49), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));

    gContactProcessedCallback = playBoing;

    Mix_PlayMusic(music, -1);
}

void PongApplication::gameOverScreen(bool win)
{
    wallWorld->pause();
    gameOver = true;
    if(win)
    {
        pause_pop_up->setText("Congratulations, you won!\n\n Press escape to exit.");
    }
    else
    {
        pause_pop_up->setText("Congratulations, you're a loser!\n\n Press escape to exit.");
    }
    pause_pop_up->setVisible(true);
}

void PongApplication::prepareMultiPlayer(void)
{
    client = (hostOption->getSelectedButtonInGroup()->getID()==1) ? true : false;
    //printf("Here's what was typed: %s\n", toConnect->getText().c_str());
    char* typedText = const_cast<char *>(toConnect->getText().c_str());
    //printf("The selected type was %s.\n", client?"client":"host");
    if(client)
        hostname = typedText;
    else
        hostname = NULL;
    //error checking
    if(client && strcmp(typedText, "")==0)
    {
        mult_info->setText("ERROR: You must enter the hostname of who you're connecting to if you're not hosting!!");
        Mix_PlayChannel(-1, error, 0);
        return;
    }
    if(!client && strcmp(typedText, "")!=0)
    {
        mult_info->setText("ERROR: If you're hosting, you shouldn't enter anything in the textbox.");
        Mix_PlayChannel(-1, error, 0);
        return;       
    }
    mult_info->setText("PLEASE WAIT\n\nWaiting for someone to connect...");
    mult_menu->setVisible(false);
    beginMultiPlayer();
}

void PongApplication::showMultiPlayerOptions(void)
{
    start_menu->setVisible(false);
    mult_menu->setVisible(true);
}

void PongApplication::beginMultiPlayer(void)
{
        multiplayer = true;
        if(SDL_Init(0) == -1)
            exit(-1);

        if(SDLNet_Init() == -1)
            exit(-1);

        if(hostname != NULL)
        {
            client = true;
            IPaddress* addr = new IPaddress();
            if(SDLNet_Init() == -1)
                exit(-1);

            if(SDLNet_ResolveHost(addr, hostname, 2025) == -1)
            {
                printf("Could not resolve hostname.\n");
                exit(-1);
            }

            printf("%s\n", hostname);

            sock = SDLNet_TCP_Open(addr);

            if(sock == NULL)
            {
                printf("Couldn't connect to server: %s", SDLNet_GetError());
                exit(-1);
            }

            uint16_t i;

            if(SDLNet_TCP_Recv(sock, &i, 2) <= 0)
            {
                printf("An error occurred when communicating with server.\n");
                exit(-1);
            }

            printf("%d\n", i);
        }
        else
        {
            IPaddress addr;
            SDLNet_ResolveHost(&addr, NULL, 2025);
            TCPsocket intermediateSock = SDLNet_TCP_Open(&addr);
            SDLNet_SocketSet sockset = SDLNet_AllocSocketSet(1);

            if(intermediateSock == NULL)
            {
                printf("Couldn't initialize server socket.");
                exit(-1);
            }
            if(SDLNet_TCP_AddSocket(sockset, intermediateSock) == -1)
            {
                printf("Error adding server socket to watch list\n");
            }
            printf("Awaiting connection...\n");
            while(true)
            {
                SDLNet_CheckSockets(sockset, 0);
                if(SDLNet_SocketReady(intermediateSock))
                {
                    sock = SDLNet_TCP_Accept(intermediateSock);
                    break;
                }
                printf("Waiting...\n");
            }
            if(sock == NULL)
            {
                printf("Error accepting connection from client.\n");
                exit(-1);
            }
            printf("Accepted connection.\n");
            SDLNet_TCP_Close(intermediateSock);
            SDLNet_TCP_Send(sock, &addr.port, 2);
        }
        createMultiPlayerScene(sock);
}


//--------------------------------------------------------------------------------------
bool PongApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(paddle != NULL && mCamera != NULL && wallWorld != NULL && ball != NULL)
    {
        if (pressedKeys.find(OIS::KC_RIGHT) != pressedKeys.end() && !wallWorld->isPaused())
        {
            paddle->translate(Ogre::Vector3((-2*(float)vel/3)*evt.timeSinceLastFrame, 0, 0));
        }
        if (pressedKeys.find(OIS::KC_LEFT) != pressedKeys.end() && !wallWorld->isPaused())
        {
            paddle->translate(Ogre::Vector3((2*(float)vel/3)*evt.timeSinceLastFrame, 0, 0));
        }
        if (pressedKeys.find(OIS::KC_UP) != pressedKeys.end() && !wallWorld->isPaused())
        {
            paddle->translate(Ogre::Vector3(0, (2*(float)vel/3)*evt.timeSinceLastFrame, 0));
        }
        if (pressedKeys.find(OIS::KC_DOWN) != pressedKeys.end() && !wallWorld->isPaused())
        {
            paddle->translate(Ogre::Vector3(0, (-2*(float)vel/3)*evt.timeSinceLastFrame, 0));
        }
        mCamera->setPosition(paddle->getPosition() + -100 * Ogre::Vector3::UNIT_Z);

        ball->setVelocity(Ogre::Vector3(ball->getVelocity().x, ball->getVelocity().y, ball->getVelocity().z < 0 ? -1*vel : vel));

        if(gameOver && !multiplayer)
        {
            wallWorld->pause();
            Mix_PauseMusic();
            pause_pop_up->setText("Your final score was: " + std::to_string(player_score) + "\n\nPress enter to play again!");
            pause_pop_up->setVisible(true);
        }
        wallWorld->update(evt.timeSinceLastFrame);
    	if(client)
    	{
        	ball->setPosition(Ogre::Vector3(-1*ballMostRecentSentPosition.x, ballMostRecentSentPosition.y, -1*ballMostRecentSentPosition.z));
        	printf("%f, %f, %f\n", ballMostRecentSentPosition.x, ballMostRecentSentPosition.y, ballMostRecentSentPosition.z);
    	}
    }
    if(client)
    {
        app.updateScoreboard();
    }
    return BaseApplication::frameRenderingQueued(evt);
}

//--------------------------------------------------------------------------------------
void PongApplication::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(0,0,-180);
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);
}

//--------------------------------------------------------------------------------------
void PongApplication::createViewports()
{
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
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
        // Create application object
        //PongApplication app;
	
	/*
        if(SDL_Init(0) == -1)
            return -1;

        if(SDLNet_Init() == -1)
            return -1;

        if(argc > 1)
        {
            client = true;
            IPaddress* addr = new IPaddress();
            if(SDLNet_Init() == -1)
                return -1;

            if(SDLNet_ResolveHost(addr, argv[1], 2025) == -1)
            {
                printf("Could not resolve hostname.\n");
                return -1;
            }

            printf("%s\n", argv[1]);

            app.sock = SDLNet_TCP_Open(addr);

            if(app.sock == NULL)
            {
                printf("Couldn't connect to server: %s", SDLNet_GetError());
                return -1;
            }

            uint16_t i;

            if(SDLNet_TCP_Recv(app.sock, &i, 2) <= 0)
            {
                printf("An error occurred when communicating with server.\n");
                return -1;
            }

            printf("%d\n", i);
        }
        else
        {
            IPaddress addr;
            SDLNet_ResolveHost(&addr, NULL, 2025);
            TCPsocket intermediateSock = SDLNet_TCP_Open(&addr);
            SDLNet_SocketSet sockset = SDLNet_AllocSocketSet(1);

            if(intermediateSock == NULL)
            {
                printf("Couldn't initialize server socket.");
                return -1;
            }
            if(SDLNet_TCP_AddSocket(sockset, intermediateSock) == -1)
            {
                printf("Error adding server socket to watch list\n");
            }
            printf("Awaiting connection...\n");
            while(true)
            {
                SDLNet_CheckSockets(sockset, 0);
                if(SDLNet_SocketReady(intermediateSock))
                {
                    app.sock = SDLNet_TCP_Accept(intermediateSock);
                    break;
                }
                printf("Waiting...\n");
            }
            if(app.sock == NULL)
            {
                printf("Error accepting connection from client.\n");
                return -1;
            }
            printf("Accepted connection.\n");
            SDLNet_TCP_Close(intermediateSock);
            SDLNet_TCP_Send(app.sock, &addr.port, 2);
        }
	*/
        if(argc > 1)
        {
            hostname = argv[1];
        }

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
