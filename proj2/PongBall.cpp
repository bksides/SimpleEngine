#include "PongBall.h"
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

PongBall::PongBall(Ogre::SceneManager* mSceneMgr, btVector3 pos)
{
    btCollisionShape* fallShape = new btSphereShape(5);

    btDefaultMotionState* fallMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), pos));
    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    rigidBody = new btRigidBody(fallRigidBodyCI);
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    rigidBody->setRestitution(1.0);

    mesh = mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
    mesh->setMaterialName("Ball/Skin");
}

void PongBall::setParentSceneNode(Ogre::SceneNode* node)
{
    GameObject::setParentSceneNode(node);
    this->node->scale(0.1, 0.1, 0.1);
}
