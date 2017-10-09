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

    dynamicsWorld->stepSimulation(deltaTime);
}

void World::addObject(GameObject* obj,
    const Ogre::Vector3& pos,
    const Ogre::Vector3& vel)
{
    obj->setParentSceneNode(mSceneMgr->getRootSceneNode());
    obj->setPosition(pos);
    obj->setVelocity(vel);
    objects.push_front(obj);

    if(obj->getRigidBody())
    {
        btVector3 physVec = btVector3(pos.x, pos.y, pos.z);
        dynamicsWorld->addRigidBody(obj->getRigidBody());
        obj->getRigidBody()->getMotionState->setTransform(physVec);
    }
}

void World::onObjectAdded(GameObject* obj,
    const Ogre::Vector3& pos,
    const Ogre::Vector3& vel) {}

void World::onUpdate(float deltaTime) {}

World::World(Ogre::SceneManager* m) : mSceneMgr(m) {

    broadphase = new btDbvtBroadphase();

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

World::~World()
{
    for(GameObject* obj : objects)
    {
        delete obj;
    }

    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}
