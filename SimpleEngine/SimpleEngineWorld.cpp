#include "SimpleEngineWorld.h"
#include "SimpleEngineGameObject.h"
#include <OgreSceneManager.h>

using namespace SimpleEngine;

void World::update(float deltaTime)
{
    onUpdate(deltaTime);

    for(GameObject* obj : objects)
    {
        obj->translate(obj->getVelocity()*deltaTime);
        obj->update(deltaTime);
    }
}

void World::addObject(GameObject* obj,
    const Ogre::Vector3& pos,
    const Ogre::Vector3& vel)
{
    obj->setParentSceneNode(mSceneMgr->getRootSceneNode());
    obj->setPosition(pos);
    obj->setVelocity(vel);
    objects.push_front(obj);
}

void World::onObjectAdded(GameObject* obj,
    const Ogre::Vector3& pos,
    const Ogre::Vector3& vel) {}

void World::onUpdate(float deltaTime) {}

World::World(Ogre::SceneManager* m) : mSceneMgr(m) {}

World::~World()
{
    for(GameObject* obj : objects)
    {
        delete obj;
    }
}
