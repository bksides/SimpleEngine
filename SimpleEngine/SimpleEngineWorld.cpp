#include "SimpleEngineWorld.h"
#include "SimpleEngineGameObject.h"
#include <OgreMath.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreSceneManager.h>
#include <thread>
#include <chrono>
#include <unistd.h>

using namespace SimpleEngine;

void World::update(float deltaTime)
{
    if(!paused)
    {
        onUpdate(deltaTime);

        for(GameObject* obj : objects)
        {
            btTransform trans;
            obj->getRigidBody()->getMotionState()->getWorldTransform(trans);
            obj->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
            btQuaternion rbOrientation = obj->getRigidBody()->getOrientation();
            btVector3 rbOrientationAxis = rbOrientation.getAxis();
            obj->setRotation(Ogre::Quaternion(Ogre::Radian(rbOrientation.getAngle()),
                Ogre::Vector3(rbOrientationAxis.x(),
                    rbOrientationAxis.y(),rbOrientationAxis.z())));
                    obj->update(deltaTime);
        }
    }
}

btDiscreteDynamicsWorld* World::getDynamicsWorld()
{
    return dynamicsWorld;
}

void physLoop(SimpleEngine::World* world)
{
    while(true)
    {
        std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
        int elapsedTime = (t - world->lastPhysicsUpdate).count();
        world->getDynamicsWorld()->stepSimulation((float)elapsedTime / 1000);
        world->lastPhysicsUpdate = t;
        usleep(16000);
    }
}

void World::addObject(GameObject* obj,
    const Ogre::Vector3& pos,
    const Ogre::Vector3& vel,
    const Ogre::Vector3& rot)
{
    obj->setParentSceneNode(mSceneMgr->getRootSceneNode());
    if(obj->getRigidBody())
    {
        dynamicsWorld->addRigidBody(obj->getRigidBody());
    }
    obj->setPosition(pos);
    obj->setVelocity(vel);
    obj->setRotation(rot);
    objects.push_front(obj);
}

void World::pause( bool pause )
{
    paused = pause;
}

bool World::isPaused()
{
    return paused;
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

    physicsThread = new std::thread(physLoop, this);

    btContactSolverInfo& info = dynamicsWorld->getSolverInfo();
    info.m_splitImpulse = 1;
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
    delete physicsThread;
}
