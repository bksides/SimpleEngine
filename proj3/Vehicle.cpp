#include "Vehicle.h"

Vehicle::Vehicle(Ogre::SceneManager* mSceneManager)
{
	mesh = mSceneManager->createEntity("avalon_chair.mesh");
	mesh->setCastShadows(true);

	btCollisionShape* groundShape = new btBoxShape(btVector3(50, 0.1, 50));

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    rigidBody = new btRigidBody(groundRigidBodyCI);
    rigidBody->setRestitution(1.0);
}