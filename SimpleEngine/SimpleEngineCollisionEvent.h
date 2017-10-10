/*! \file SimpleEngine/SimpleEngineCollisionEvent.h
    \brief A file describing the SimpleEngine::CollisionEvent class.
 */

#ifndef SIMPLE_ENGINE_COLLISION_EVENT_H
#define SIMPLE_ENGINE_COLLISION_EVENT_H

#include "SimpleEngineGameObject.h"
#include <OgreVector3.h>

/*!
 \brief Namespace containing several useful game-related classes.
 */
namespace SimpleEngine
{
    class GameObject;

    /*!
     \brief Class containing information about a particular collision.

     Upon each collision between GameObjects, the onCollision callback is
     called for each object involved in the collision with a CollisionEvent
     parameter describing the collision.
     */
    struct CollisionEvent
    {
        /*!
         \brief A pointer to the GameObject with which this GameObject collided.
         */
        SimpleEngine::GameObject* collider;
        /*!
         \brief A vector normal to the plane of collision.
         */
        Ogre::Vector3 normal;
        CollisionEvent(SimpleEngine::GameObject* collider, Ogre::Vector3 normal);
    };
};

#endif
