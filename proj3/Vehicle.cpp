#include "Vehicle.h"
#include <cstdio>

Vehicle::Vehicle(Ogre::SceneManager* mSceneManager)
{
    printf("In vehicle constructor.");
	mesh = mSceneManager->createEntity("penguin.mesh");
    printf("Loaded mesh\n");
	mesh->setCastShadows(true);
    mesh->setMaterialName("Penguin");
    printf("Did some shit to mesh\n");

	btCollisionShape* chairShape = new btBoxShape(btVector3(1, 2.3, 1));

    btDefaultMotionState* chairMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    btScalar mass = 1;
    btVector3 chairInertia(0, 0, 0);
    chairShape->calculateLocalInertia(mass, chairInertia);
    btRigidBody::btRigidBodyConstructionInfo chairRigidBodyCI(mass, chairMotionState, chairShape, chairInertia);
    rigidBody = new btRigidBody(chairRigidBodyCI);
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags());
    rigidBody->setRestitution(.2);
    printf("Made it to end of constructor\n");
}

void Vehicle::setParentSceneNode(Ogre::SceneNode* node)
{
    GameObject::setParentSceneNode(node);
    this->posnode->scale(0.1, 0.1, 0.1);
}