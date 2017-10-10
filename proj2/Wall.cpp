#include "Wall.h"
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>

Wall::Wall(Ogre::SceneManager* mSceneMgr)
{
    mesh = mSceneMgr->createEntity("wall");
    mesh->setMaterialName("Examples/Rockwall");

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    rigidBody = new btRigidBody(groundRigidBodyCI);
    rigidBody->setRestitution(1.0);
}
