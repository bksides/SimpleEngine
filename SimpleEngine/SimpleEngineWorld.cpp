#include "SimpleEngineWorld.h"
#include "SimpleEngineGameObject.h"
#include <OgreSceneManager.h>

using namespace SimpleEngine;

void World::update(float deltaTime)
{
    onUpdate(deltaTime);

    dynamicsWorld->stepSimulation(deltaTime);

    for(GameObject* obj : objects)
    {
        btTransform trans;
        obj->getRigidBody()->getMotionState()->getWorldTransform(trans);
        obj->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
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

    if(obj->getRigidBody())
    {
        btVector3 physPos = btVector3(pos.x, pos.y, pos.z);
        btVector3 physVel = btVector3(vel.x, vel.y, vel.z);
        dynamicsWorld->addRigidBody(obj->getRigidBody());
        obj->getRigidBody()->getMotionState()->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), physPos));
        obj->getRigidBody()->setLinearVelocity(physVel);
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

    dynamicsWorld->setGravity(btVector3(0, -100, 0));
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
