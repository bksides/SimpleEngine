#include "BallApplication.h"
#include "Ball.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <OgreMeshManager.h>

//-------------------------------------------------------------------------------------
BallApplication::BallApplication(void) : ballWorld(nullptr)
{
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
    Ball::boing = Mix_LoadWAV( "./dist/media/sounds/boing.wav" );
    if( Ball::boing == NULL )
    {
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        mShutDown = true;
    }
}
//-------------------------------------------------------------------------------------
BallApplication::~BallApplication(void)
{
    Mix_FreeChunk(Ball::boing);
    Mix_CloseAudio();
    SDL_Quit();
    delete ballWorld;
}

//-------------------------------------------------------------------------------------
void BallApplication::createScene(void)
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

    /**
    ---WALLS---
    **/

    //Set up wall mesh
    Ogre::Plane wallPlane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "wall",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        wallPlane,
        100,100,20,20,
        true,
        1,5,5,
        Ogre::Vector3::UNIT_Z);

    //Create wall entities
    Ogre::Entity* floorEntity = mSceneMgr->createEntity("wall");
    Ogre::Entity* ceilingEntity = mSceneMgr->createEntity("wall");
    Ogre::Entity* leftWallEntity = mSceneMgr->createEntity("wall");
    Ogre::Entity* rightWallEntity = mSceneMgr->createEntity("wall");
    Ogre::Entity* frontWallEntity = mSceneMgr->createEntity("wall");
    Ogre::Entity* backWallEntity = mSceneMgr->createEntity("wall");

    //Set wall materials and shadow properties
    floorEntity->setMaterialName("Examples/Rockwall");
    floorEntity->setCastShadows(false);
    ceilingEntity->setMaterialName("Examples/Rockwall");
    ceilingEntity->setCastShadows(false);
    leftWallEntity->setMaterialName("Examples/Rockwall");
    leftWallEntity->setCastShadows(false);
    rightWallEntity->setMaterialName("Examples/Rockwall");
    rightWallEntity->setCastShadows(false);
    frontWallEntity->setMaterialName("Examples/Rockwall");
    frontWallEntity->setCastShadows(false);
    backWallEntity->setMaterialName("Examples/Rockwall");
    backWallEntity->setCastShadows(false);

    //Create wall nodes
    Ogre::SceneNode* floorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(0,-50,0));
    Ogre::SceneNode* ceilingNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(0,50,0));
    Ogre::SceneNode* leftWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(-50,0,0));
    Ogre::SceneNode* rightWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(50,0,0));
    Ogre::SceneNode* frontWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(0,0,50));
    Ogre::SceneNode* backWallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(
        Ogre::Vector3(0,0,-50));

    //Rotate wall nodes
    ceilingNode->pitch(Ogre::Degree(180));
    frontWallNode->pitch(Ogre::Degree(-90));
    backWallNode->pitch(Ogre::Degree(90));
    leftWallNode->roll(Ogre::Degree(-90));
    leftWallNode->yaw(Ogre::Degree(90));
    rightWallNode->roll(Ogre::Degree(90));
    rightWallNode->yaw(Ogre::Degree(90));

    //Attach wall entities to nodes
    floorNode->attachObject(floorEntity);
    ceilingNode->attachObject(ceilingEntity);
    frontWallNode->attachObject(frontWallEntity);
    backWallNode->attachObject(backWallEntity);
    leftWallNode->attachObject(leftWallEntity);
    rightWallNode->attachObject(rightWallEntity);

    ballWorld = new BallWorld(50, Ogre::Vector3(0, -98, 0), mSceneMgr);

    ballWorld->addObject((SimpleEngine::GameObject*)(new Ball(mSceneMgr, 5)));
    ballWorld->addObject((SimpleEngine::GameObject*)(new Ball(mSceneMgr, 5)), Ogre::Vector3( 40,  40, 0));
    ballWorld->addObject((SimpleEngine::GameObject*)(new Ball(mSceneMgr, 5)), Ogre::Vector3(-40,  40, 0));
    ballWorld->addObject((SimpleEngine::GameObject*)(new Ball(mSceneMgr, 5)), Ogre::Vector3(-40, -40, 0));
    ballWorld->addObject((SimpleEngine::GameObject*)(new Ball(mSceneMgr, 5)), Ogre::Vector3( 40, -40, 0));
}

//--------------------------------------------------------------------------------------
bool BallApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    ballWorld->update(evt.timeSinceLastFrame);
    return BaseApplication::frameRenderingQueued(evt);
}

//--------------------------------------------------------------------------------------
void BallApplication::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(0,0,-150);
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);
}

//--------------------------------------------------------------------------------------
void BallApplication::createViewports()
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
        BallApplication app;

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
