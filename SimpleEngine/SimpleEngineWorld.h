/*! \file SimpleEngineWorld.h
    \brief A file describing the SimpleEngine::World class
    */

#ifndef SIMPLE_ENGINE_WORLD_H
#define SIMPLE_ENGINE_WORLD_H

#include "SimpleEngineGameObject.h"
#include <OgreSceneManager.h>
#include <list>
#include <btBulletDynamicsCommon.h>
#include <chrono>
#include <thread>

namespace SimpleEngine
{
    /*!
    \brief A class representing the game world.

    This class handles the scene, the physics world, game updates, and
    interactions between objects.
    */
    class World
    {
    private:
        Ogre::SceneManager* mSceneMgr = NULL;
        btBroadphaseInterface* broadphase = NULL;
        btDefaultCollisionConfiguration* collisionConfiguration = NULL;
        btCollisionDispatcher* dispatcher = NULL;
        btSequentialImpulseConstraintSolver* solver = NULL;
        btDiscreteDynamicsWorld* dynamicsWorld = NULL;
        bool paused = false;
        std::thread* physicsThread = NULL;
    protected:
        /*!
        \brief A list of all objects in the world.
        */
        std::list<GameObject*> objects;

        /*!
        \brief A callback function invoked on updates to the world.

        This function is declared virtual so that derived classes can implement
        specific update functionality.

        \param deltaTime The time elapsed since the last world update.
        */
        virtual void onUpdate(float deltaTime);

        /*!
        \brief A callback function invoked when an object is added to the world.

        This function is declared virtual so that derived classes can implement
        specific functionality for when objects are added.

        \param obj A pointer to the GameObject which has been added to the
        world.

        \param pos The global position at which the object has been added.

        \param vel The initial velocity of this object.
        */
        virtual void onObjectAdded(GameObject* obj,
            const Ogre::Vector3& pos=Ogre::Vector3::ZERO,
            const Ogre::Vector3& vel=Ogre::Vector3::ZERO);
    public:
        /*!
        \brief A function which handles all update logic for the World.

        The onUpdate(deltaTime) function is called from here.

        \param deltaTime The time elapsed since the last update.
        */
        void update(float deltaTime);

        /*!
        \brief A function that either pauses or unpauses the World.

        \param pause If true, this function pauses the world; if false, it
        unpauses the world.
        */
        void pause(bool pause = true);

        /*!
        \brief Returns true if the World is paused; false otherwise.
        */

        bool isPaused();

        btDiscreteDynamicsWorld* getDynamicsWorld();

        /*!
        \brief A function which handles adding objects to the world.

        The onObjectAdded(obj, pos, vel) function is called from here.

        \param obj A pointer to the object to be added to the world.

        \param pos The position at which to add the GameObject.

        \param vel The initial velocity of the object.
        */
        void addObject(GameObject* obj,
            const Ogre::Vector3& pos=Ogre::Vector3::ZERO,
            const Ogre::Vector3& vel=Ogre::Vector3::ZERO,
            const Ogre::Vector3& rot=Ogre::Vector3::ZERO);

        /*!
        \param m The Ogre::SceneManager which will render all the GameObjects
        in this World.
        */
        World(Ogre::SceneManager* m);

        ~World();

        std::chrono::high_resolution_clock::time_point lastPhysicsUpdate = std::chrono::high_resolution_clock::now();
    };
}

#endif
