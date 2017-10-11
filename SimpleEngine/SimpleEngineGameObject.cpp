#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreNode.h>
#include <OgreEntity.h>
#include <OgreMath.h>
#include <OgreQuaternion.h>
#include <btBulletDynamicsCommon.h>
#include "SimpleEngineGameObject.h"
#include "SimpleEngineCollisionEvent.h"

using namespace SimpleEngine;

void GameObject::addVelocity(const Ogre::Vector3& vel)
{
    setVelocity(getVelocity()+vel);
}

Ogre::SceneNode* GameObject::getSceneNode()
{
    return node;
}

void GameObject::setVelocity(const Ogre::Vector3& vel)
{
    rigidBody->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}

const Ogre::Vector3 GameObject::getVelocity()
{
    btVector3 btvec = rigidBody->getLinearVelocity();
    return Ogre::Vector3(btvec.x(), btvec.y(), btvec.z());
}

const Ogre::Vector3 GameObject::getPosition()
{
    return node->getPosition();
}

void GameObject::translate(const Ogre::Vector3& vec)
{
    node->translate(vec);

    rigidBody->translate(btVector3(vec.x, vec.y, vec.z));
}

void GameObject::setPosition(const Ogre::Vector3& pos)
{
    node->setPosition(pos);
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
}

void GameObject::setRotation(const Ogre::Vector3& rot)
{
    node->resetOrientation();
    node->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(rot.x));
    node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(rot.y));
    node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(rot.z));
}

void GameObject::rotate(const Ogre::Vector3& rot)
{
    node->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(rot.x), Ogre::Node::TS_WORLD);
    node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(rot.y), Ogre::Node::TS_WORLD);
    node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(rot.z), Ogre::Node::TS_WORLD);

    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);

    trans.setRotation(btQuaternion(btVector3(1, 0, 0), rot.x));
    trans.setRotation(btQuaternion(btVector3(0, 1, 0), rot.y));
    trans.setRotation(btQuaternion(btVector3(0, 0, 1), rot.z));
}

const Ogre::Quaternion GameObject::getRotation()
{
    btQuaternion btquat = rigidBody->getOrientation();
    btVector3 btaxis = btquat.getAxis();
    return Ogre::Quaternion(Ogre::Radian(btquat.getAngle()), Ogre::Vector3(btaxis.x(), btaxis.y(), btaxis.z()));
}

void GameObject::update(float deltaTime)
{
    onUpdate(deltaTime);
}

btRigidBody* GameObject::getRigidBody()
{
    return rigidBody;
}

void GameObject::onUpdate(float deltaTime) {}

void GameObject::onCollision(const CollisionEvent& evt) {}

GameObject::~GameObject()
{
    node->getParentSceneNode()->removeChild((Ogre::Node*)node);
    delete rigidBody->getMotionState();
    delete rigidBody;
}

void GameObject::setParentSceneNode(Ogre::SceneNode* sceneNode)
{
    node = sceneNode->createChildSceneNode();
    node->attachObject(mesh);
}

GameObject::GameObject(Ogre::Entity* mesh, btRigidBody* rb) : mesh(mesh), rigidBody(rb) {}

GameObject::GameObject() {}
