#include "StartTile.h"
#include "FloorTile.h"
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>

StartTile::StartTile(Ogre::SceneManager* mSceneMgr) : FloorTile(mSceneMgr)
{
    Ogre::Plane startPlane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "start",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        startPlane,
        100,100,20,20,
        true,
        1,5,5,
        Ogre::Vector3::UNIT_Z);

    mesh = mSceneMgr->createEntity("start");
    mesh->setMaterialName("Terrain/Checker");

    btCollisionShape* groundShape = new btBoxShape(btVector3(50, 0.1, 50));

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    rigidBody = new btRigidBody(groundRigidBodyCI);
    rigidBody->setRestitution(1.0);
}