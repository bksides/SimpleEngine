#include "FloorTile.h"
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>

FloorTile::FloorTile(Ogre::SceneManager* mSceneMgr)
{
    Ogre::Plane floorPlane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "floor",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        floorPlane,
        100,100,20,20,
        true,
        1,5,5,
        Ogre::Vector3::UNIT_Z);

    mesh = mSceneMgr->createEntity("floor");
    mesh->setMaterialName("Terrain/Rockdirt");

    btCollisionShape* groundShape = new btBoxShape(btVector3(50, 0.1, 50));

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    rigidBody = new btRigidBody(groundRigidBodyCI);
    rigidBody->setRestitution(1.0);
}