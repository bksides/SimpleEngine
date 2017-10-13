#include "Paddle.h"
#include <btBulletDynamicsCommon.h>
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <cstdio>

Paddle::Paddle(Ogre::SceneManager* mSceneMgr)
{
	printf("In constructor\n");
	if(!mSceneMgr)
	{
		printf("No SceneManager");
	}
	printf("Got SceneManager\n");
    Ogre::Plane paddlePlane(Ogre::Vector3::UNIT_Y, 0);
    printf("Constructed paddlePlane");
    Ogre::MeshManager::getSingleton().createPlane(
        "paddle",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        paddlePlane,
        10,10,20,20,
        true,
        1,5,5,
        Ogre::Vector3::UNIT_Z);
    printf("Created plane");
    mesh = mSceneMgr->createEntity("paddle");
    if(!mesh)
    {
    	printf("no mesh\n");
    }
    printf("Got mesh\n");
    mesh->setMaterialName("Ball/Skin");

    btCollisionShape* paddleShape = new btBoxShape(btVector3(5, 0.1, 5));
    if(!paddleShape)
    {
    	printf("No shape\n");
    }
    printf("Got shape\n");

    btDefaultMotionState* paddleMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 50)));
    if(!paddleMotionState)
    {
    	printf("No motion state\n");
    }
    printf("Got motion state\n");
    btRigidBody::btRigidBodyConstructionInfo paddleRigidBodyCI(0, paddleMotionState, paddleShape, btVector3(0, 0, 0));
    rigidBody = new btRigidBody(paddleRigidBodyCI);
    if(!rigidBody)
    {
    	printf("No rigid body\n");
    }
    printf("Got rigid body\n");
    rigidBody->setRestitution(1.3);
    printf("Finished constructor\n");
}