#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreNode.h>
#include <OgreEntity.h>
#include "SimpleEngineGameObject.h"
#include "SimpleEngineCollisionEvent.h"

using namespace SimpleEngine;

void GameObject::addVelocity(const Ogre::Vector3& vel)
{
    setVelocity(getVelocity()+vel);
}

void GameObject::setVelocity(const Ogre::Vector3& vel)
{
    velocity = vel;
}

const Ogre::Vector3& GameObject::getVelocity()
{
    return velocity;
}

const Ogre::Vector3& GameObject::getPosition()
{
    if(node != NULL)
    {
        return node->getPosition();
    }
    else
    {
        return Ogre::Vector3(0, 0, 0);
    }
}

void GameObject::translate(const Ogre::Vector3& vec)
{
    node->translate(vec);
}

void GameObject::setPosition(const Ogre::Vector3& pos)
{
    node->setPosition(pos);
}

void GameObject::update(float deltaTime)
{
    onUpdate(deltaTime);
}

void GameObject::onUpdate(float deltaTime) {}

void GameObject::onCollision(const CollisionEvent& evt) {}

GameObject::~GameObject()
{
    node->getParentSceneNode()->removeChild((Ogre::Node*)node);
}

void GameObject::setParentSceneNode(Ogre::SceneNode* sceneNode)
{
    node = sceneNode->createChildSceneNode();
    node->attachObject(mesh);
}

GameObject::GameObject(Ogre::Entity* mesh) : mesh(mesh) {}
