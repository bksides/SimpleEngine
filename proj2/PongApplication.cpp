#include "PongApplication.h"
#include "Paddle.h"
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

using namespace SimpleEngine;

Mix_Chunk* boing = NULL;
GameObject* paddle;
GameObject* ball;
int vel = 30;
int player_score = 0;
PongApplication app;
CEGUI::Window *score_board;
CEGUI::Window *pause_pop_up;
bool gameOver = false;
bool CEGUI_needs_init = true;

bool playBoing(btManifoldPoint& cp, void* body0, void* body1)
{
    if(sound)
    {
        Mix_PlayChannel( -1, boing, 0 );
    }
    if((body0 == ball->getRigidBody() && body1 == paddle->getRigidBody())||
        (body1 == ball->getRigidBody() && body0 == paddle->getRigidBody()))
    {
        ++player_score;
        //quit->setText(std::to_string(player_score));
        app.updateScoreboard();
        vel += 3;
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
    if( boing == NULL )
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
    score_board->setText("Score: "+std::to_string(player_score));
}

void PongApplication::createScene(void)
{

	//Here we should initialize the PongWorld and populate it with GameObjects
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

    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO);
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / 2));
    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / -2));
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI));
	wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));

    ball = new PongBall(mSceneMgr, btVector3(0,0,0));

    wallWorld->addObject(ball, Ogre::Vector3::ZERO, Ogre::Vector3(Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(40, 40), Ogre::Math::RangeRandom(40, 40)));

    paddle = new Paddle(mSceneMgr);
    wallWorld->addObject(paddle, Ogre::Vector3(0, 0, -49), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));

    gContactProcessedCallback = playBoing;

    if(CEGUI_needs_init)
    {
        CEGUI_Init();
        CEGUI_needs_init = false;
    }
}

bool PongApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if(arg.key == OIS::KC_P)
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
    if(arg.key == OIS::KC_RETURN && wallWorld->isPaused())
    {
        vel = 30;
        player_score = 0;
        score_board->setText("Score: "+std::to_string(player_score));
        gameOver = false;
        mSceneMgr->clearScene();
        createScene();
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
    //CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

/*    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);*/

/*  Use this if you want a cool demo of what CEGUI can do with sheets
    CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TextDemo.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);
*/
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    score_board = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/ScoreBoard");

    pause_pop_up = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/PausePopUp");
    //quit->setText("Score");

    score_board->setText("Score: "+std::to_string(player_score));
    score_board->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(0, 0)));
    score_board->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

    pause_pop_up->setText("Game Paused.\n\nPress enter to start over.");
    pause_pop_up->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(.35, 0)));
    pause_pop_up->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.3, 0)));
    pause_pop_up->setVisible(false);

    sheet->addChild(score_board);
    sheet->addChild(pause_pop_up);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}


//--------------------------------------------------------------------------------------
bool PongApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
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

    ball->setVelocity(Ogre::Vector3(ball->getVelocity().x, ball->getVelocity().y, (ball->getVelocity().z * ((float)vel / abs(ball->getVelocity().z)))));
    if(ball->getPosition().z < -50)
    {
        score_board->setText("Final Score: " + std::to_string(player_score));
        gameOver = true;
    }
    if(gameOver)
    {
        wallWorld->pause();

    }
    wallWorld->update(evt.timeSinceLastFrame);
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
