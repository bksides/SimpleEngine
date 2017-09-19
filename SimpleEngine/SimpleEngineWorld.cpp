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

void World::addObject(GameObject* obj, float x, float y, float z)
{
    obj->setParentSceneNode(mSceneMgr->getRootSceneNode());
    obj->setPosition(Ogre::Vector3(x, y, z));
    objects.push_front(obj);
}

void World::onObjectAdded(GameObject* obj, float x, float y, float z) {}

void World::onUpdate(float deltaTime) {}

World::World(Ogre::SceneManager* m) : mSceneMgr(m) {}

World::~World()
{
    for(GameObject* obj : objects)
    {
        delete obj;
    }
}
