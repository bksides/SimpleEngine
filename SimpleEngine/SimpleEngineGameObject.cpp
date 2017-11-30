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
    return posnode;
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
    return posnode->getPosition();
}

void GameObject::translate(const Ogre::Vector3& vec)
{
    setPosition(getPosition() + vec);
}

void GameObject::setPosition(const Ogre::Vector3& pos)
{
    posnode->setPosition(pos);

    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
    rigidBody->setWorldTransform(trans);
    rigidBody->getMotionState()->setWorldTransform(trans);
}

void GameObject::setRotation(const Ogre::Vector3& rot)
{
    rotnode->resetOrientation();
    rotnode->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(rot.x), Ogre::Node::TS_WORLD);
    rotnode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(rot.y), Ogre::Node::TS_WORLD);
    rotnode->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(rot.z), Ogre::Node::TS_WORLD);

    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);

    trans.setRotation(btQuaternion(btVector3(1, 0, 0), rot.x));
    trans.setRotation(btQuaternion(btVector3(0, 1, 0), rot.y) * trans.getRotation());
    trans.setRotation(btQuaternion(btVector3(0, 0, 1), rot.z) * trans.getRotation());

	rigidBody->setWorldTransform(trans);
	rigidBody->getMotionState()->setWorldTransform(trans);
}

void GameObject::setRotation(const Ogre::Quaternion& rot)
{
    rotnode->resetOrientation();
    rotnode->setOrientation(rot);

    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    Ogre::Radian angle;
    Ogre::Vector3 axis;
    rot.ToAngleAxis(angle, axis);

    trans.setRotation(btQuaternion(btVector3(axis.x, axis.y, axis.z), angle.valueRadians()));
}

void GameObject::rotate(const Ogre::Vector3& rot)
{
    rotnode->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(rot.x), Ogre::Node::TS_WORLD);
    rotnode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(rot.y), Ogre::Node::TS_WORLD);
    rotnode->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(rot.z), Ogre::Node::TS_WORLD);

    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);

    btQuaternion btrot = btQuaternion(btVector3(1, 0, 0), rot.x) *
                            btQuaternion(btVector3(0, 1, 0), rot.y) *
                            btQuaternion(btVector3(0, 0, 1), rot.z);

    trans.setRotation(trans.getRotation() * btrot);
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
    posnode->getParentSceneNode()->removeChild((Ogre::Node*)posnode);
    delete rigidBody->getMotionState();
    delete rigidBody;
}

void GameObject::setParentSceneNode(Ogre::SceneNode* sceneNode)
{
    posnode = sceneNode->createChildSceneNode();
    rotnode = posnode->createChildSceneNode();
    rotnode->attachObject(mesh);
}

GameObject::GameObject(Ogre::Entity* mesh, btRigidBody* rb) : mesh(mesh), rigidBody(rb) {}

GameObject::GameObject() {}
