#ifndef SIMPLE_ENGINE_WORLD_H
#define SIMPLE_ENGINE_WORLD_H

#include "SimpleEngineGameObject.h"
#include <OgreSceneManager.h>
#include <list>

namespace SimpleEngine
{
    class World
    {
    private:
        Ogre::SceneManager* mSceneMgr;
    protected:
        std::list<GameObject*> objects;
        virtual void onUpdate(float deltaTime);
        virtual void onObjectAdded(GameObject* obj, float x, float y, float z);
    public:
        void update(float deltaTime);
        void addObject(GameObject* obj, float x, float y, float z);
        World(Ogre::SceneManager* m);
        ~World();
    };
}

#endif
