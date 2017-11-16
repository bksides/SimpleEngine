#include "Vehicle.h"

Vehicle::Vehicle(Ogre::SceneManager* mSceneManager)
{
	mesh = mSceneManager->createEntity(Ogre::SceneManager::PT_SPHERE);
	mesh->setCastShadows(true);
    mesh->setMaterialName("Ball/Skin");

	btCollisionShape* chairShape = new btBoxShape(btVector3(50, 0.1, 50));

    btDefaultMotionState* chairMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    btScalar mass = 1;
    btVector3 chairInertia(0, 0, 0);
    chairShape->calculateLocalInertia(mass, chairInertia);
    btRigidBody::btRigidBodyConstructionInfo chairRigidBodyCI(mass, chairMotionState, chairShape, chairInertia);
    rigidBody = new btRigidBody(chairRigidBodyCI);
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    rigidBody->setRestitution(1.0);
}