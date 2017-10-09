#ifndef SIMPLE_ENGINE_WORLD_H
#define SIMPLE_ENGINE_WORLD_H

#include "SimpleEngineGameObject.h"
#include <OgreSceneManager.h>
#include <list>
#include <btBulletDynamicsCommon.h>

namespace SimpleEngine
{
    class World
    {
    private:
        Ogre::SceneManager* mSceneMgr = NULL;
        btBroadphaseInterface* broadphase = NULL;
        btDefaultCollisionConfiguration* collisionConfiguration = NULL;
        btCollisionDispatcher* dispatcher = NULL;
        btSequentialImpulseConstraintSolver* solver = NULL;
        btDiscreteDynamicsWorld* dynamicsWorld = NULL;
    protected:
        std::list<GameObject*> objects;
        virtual void onUpdate(float deltaTime);
        virtual void onObjectAdded(GameObject* obj,
            const Ogre::Vector3& pos=Ogre::Vector3::ZERO,
            const Ogre::Vector3& vel=Ogre::Vector3::ZERO);
    public:
        void update(float deltaTime);
        void addObject(GameObject* obj,
            const Ogre::Vector3& pos=Ogre::Vector3::ZERO,
            const Ogre::Vector3& vel=Ogre::Vector3::ZERO);
        World(Ogre::SceneManager* m);
        ~World();
    };
}

#endif
