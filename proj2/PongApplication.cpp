#include "PongApplication.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <OgreMeshManager.h>
#include <OgreMath.h>
#include <OISKeyboard.h>

//-------------------------------------------------------------------------------------
PongApplication::PongApplication(void)
{

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
}

//--------------------------------------------------------------------------------------
bool PongApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    return BaseApplication::frameRenderingQueued(evt);
}

//--------------------------------------------------------------------------------------
void PongApplication::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(0,0,-150);
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
