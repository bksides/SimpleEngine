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

bool playBoing(btManifoldPoint& cp, void* body0, void* body1)
{
    Mix_PlayChannel( -1, boing, 0 );
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
    //mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
}
//-------------------------------------------------------------------------------------
PongApplication::~PongApplication(void)
{
	//Destructor; time to delete pointers, free SDL structures, etc.
}

//-------------------------------------------------------------------------------------
void PongApplication::createScene(void)
{
	//Here we should initialize the PongWorld and populate it with GameObjects
    Ogre::Light* lamp = mSceneMgr->createLight("lamp");
    lamp->setType(Ogre::Light::LT_POINT);
    lamp->setPosition(0,49,0);
    lamp->setDiffuseColour(1,1,1);
    lamp->setSpecularColour(1,1,1);
    lamp->setAttenuation(200, 0, 0, .0002);

    //Create wall entities
    wallWorld = new World(mSceneMgr);

    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO);
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / 2));
    wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_X, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI / -2));
    wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO, Ogre::Vector3(0, 0, M_PI));
	wallWorld->addObject(new Wall(mSceneMgr),  50*Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / -2, 0, 0));
	wallWorld->addObject(new Wall(mSceneMgr), -50*Ogre::Vector3::UNIT_Z, Ogre::Vector3::ZERO, Ogre::Vector3(M_PI / 2, 0, 0));

    wallWorld->addObject(new PongBall(mSceneMgr, btVector3(0,0,0)), Ogre::Vector3::ZERO, Ogre::Vector3(Ogre::Math::RangeRandom(-40, 40), Ogre::Math::RangeRandom(40, 40), Ogre::Math::RangeRandom(40, 40)));

    wallWorld->addObject(new Paddle(mSceneMgr), Ogre::Vector3(0, 0, 0));

    gContactProcessedCallback = playBoing;
}

//--------------------------------------------------------------------------------------
bool PongApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    wallWorld->update(evt.timeSinceLastFrame);
    return BaseApplication::frameRenderingQueued(evt);
}

//--------------------------------------------------------------------------------------
void PongApplication::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(0,20,-150);
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
        PongApplication app;

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
