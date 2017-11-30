#include "Wall.h"
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>

Wall::Wall(Ogre::SceneManager* mSceneMgr)
{
    Ogre::Plane wallPlane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "wall",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        wallPlane,
        100,100,20,20,
        true,
        1,5,5,
        Ogre::Vector3::UNIT_Z);

    mesh = mSceneMgr->createEntity("wall");
    mesh->setMaterialName("Examples/Rockwall");

    btCollisionShape* groundShape = new btBoxShape(btVector3(50, 0.5, 50));

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    rigidBody = new btRigidBody(groundRigidBodyCI);
    rigidBody->setRestitution(1.0);
}