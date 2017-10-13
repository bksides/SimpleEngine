#include "Paddle.h"
#include <btBulletDynamicsCommon.h>
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <cstdio>

Paddle::Paddle(Ogre::SceneManager* mSceneMgr)
{
    Ogre::Plane paddlePlane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "paddle",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        paddlePlane,
        10,10,20,20,
        true,
        1,5,5,
        Ogre::Vector3::UNIT_Z);
    mesh = mSceneMgr->createEntity("paddle");
    mesh->setMaterialName("Ball/Skin");

    btCollisionShape* paddleShape = new btBoxShape(btVector3(5, 0.1, 5));

    btDefaultMotionState* paddleMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 50)));printf("No motion state\n");
    btRigidBody::btRigidBodyConstructionInfo paddleRigidBodyCI(0, paddleMotionState, paddleShape, btVector3(0, 0, 0));
    rigidBody = new btRigidBody(paddleRigidBodyCI);
    rigidBody->setRestitution(1.3);
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
}