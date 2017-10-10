#include "PongApplication.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <OgreMeshManager.h>
#include <OgreMath.h>
#include <OISKeyboard.h>

using namespace SimpleEngine;

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
    Ogre::Light* lamp = mSceneMgr->createLight("lamp");
    lamp->setType(Ogre::Light::LT_POINT);
    lamp->setPosition(0,100,0);
    lamp->setDiffuseColour(1,1,1);
    lamp->setSpecularColour(1,1,1);
    lamp->setAttenuation(200, 0, 0, .0002);

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
    floorEntity->setMaterialName("Examples/Rockwall");

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    groundRigidBody->setRestitution(1.0);

    GameObject* groundObject = new GameObject(floorEntity, groundRigidBody);
    wallWorld = new World(mSceneMgr);

    wallWorld->addObject(groundObject, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);

    btCollisionShape* fallShape = new btSphereShape(5);

    btDefaultMotionState* fallMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    fallRigidBody->setRestitution(1.05);

    Ogre::Entity* ballEntity = mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
    ballEntity->setMaterialName("Ball/Skin");

    GameObject* ballObject = new GameObject(ballEntity, fallRigidBody);
    wallWorld->addObject(ballObject, Ogre::Vector3(0,10,0), Ogre::Vector3::ZERO);
    ballObject->getSceneNode()->scale(0.1, 0.1, 0.1);


    Ogre::Entity* ceilingEntity = mSceneMgr->createEntity("wall");
    Ogre::Entity* leftWallEntity = mSceneMgr->createEntity("wall");
    Ogre::Entity* rightWallEntity = mSceneMgr->createEntity("wall");
    Ogre::Entity* frontWallEntity = mSceneMgr->createEntity("wall");
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
    mCamera->setPosition(0,50,-150);
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
