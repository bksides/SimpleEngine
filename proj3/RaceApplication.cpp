#include "Vehicle.h"
#include "RaceApplication.h"
#include "FloorTile.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <OgreMeshManager.h>
#include <OgreMath.h>
#include <OISKeyboard.h>

//-------------------------------------------------------------------------------------
RaceApplication::RaceApplication(void)
{
    
}
//-------------------------------------------------------------------------------------
RaceApplication::~RaceApplication(void)
{
    delete raceWorld;
}

//-------------------------------------------------------------------------------------
void RaceApplication::createScene(void)
{

    /**
    ---SKYBOX---
    **/

    mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

    /**
    ---LIGHTING---
    **/

    //Create room light
    Ogre::Light* lamp = mSceneMgr->createLight("lamp");
    lamp->setType(Ogre::Light::LT_POINT);
    lamp->setPosition(0,49,0);
    lamp->setDiffuseColour(1,1,1);
    lamp->setSpecularColour(1,1,1);
    lamp->setAttenuation(200, 0, 0, .0002);

    //Create ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3,0.3,0.3));

    //Set shadow rendering mode
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    raceWorld = new RaceWorld(mSceneMgr);

    raceWorld->addObject(new FloorTile(mSceneMgr));
    raceWorld->addObject(new Vehicle(mSceneMgr), Ogre::Vector3::UNIT_Y*10);
}

//--------------------------------------------------------------------------------------
bool RaceApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (pressedKeys.find(OIS::KC_RIGHT) != pressedKeys.end())
	{
		mCamera->setPosition(mCamera->getPosition() + 20*mCamera->getRight()*evt.timeSinceLastFrame);
	}
	if (pressedKeys.find(OIS::KC_LEFT) != pressedKeys.end())
	{
		mCamera->setPosition(mCamera->getPosition() + -20*mCamera->getRight()*evt.timeSinceLastFrame);
	}
	if (pressedKeys.find(OIS::KC_UP) != pressedKeys.end())
	{
		mCamera->setPosition(mCamera->getPosition() + 20*mCamera->getDirection()*evt.timeSinceLastFrame);
	}
	if (pressedKeys.find(OIS::KC_DOWN) != pressedKeys.end())
	{
		mCamera->setPosition(mCamera->getPosition() + -20*mCamera->getDirection()*evt.timeSinceLastFrame);
	}


    raceWorld->update(evt.timeSinceLastFrame);
    return BaseApplication::frameRenderingQueued(evt);
}

//--------------------------------------------------------------------------------------
void RaceApplication::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(0,40,100);
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);
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
