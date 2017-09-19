#ifndef SIMPLE_ENGINE_COLLISION_EVENT_H
#define SIMPLE_ENGINE_COLLISION_EVENT_H

#include "SimpleEngineGameObject.h"
#include <OgreVector3.h>

namespace SimpleEngine
{
    class GameObject;

    struct CollisionEvent
    {
        SimpleEngine::GameObject* collider;
        Ogre::Vector3 normal;
        CollisionEvent(SimpleEngine::GameObject* collider, Ogre::Vector3 normal);
    };
};

#endif
