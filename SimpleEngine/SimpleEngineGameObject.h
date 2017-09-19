#ifndef SIMPLE_ENGINE_GAME_OBJECT_H
#define SIMPLE_ENGINE_GAME_OBJECT_H

#include <OgreSceneNode.h>
#include <OgreVector3.h>
#include <OgreEntity.h>
#include <OgreMaterial.h>
#include "SimpleEngineCollisionEvent.h"

namespace SimpleEngine
{
    class GameObject
    {
    protected:
        Ogre::Vector3 velocity;
        Ogre::SceneNode* node;
        Ogre::Entity* mesh;
        virtual void onUpdate(float deltaTime);
    public:
        void update(float deltaTime);
        const Ogre::Vector3& getVelocity();
        void addVelocity(const Ogre::Vector3& vel);
        void setVelocity(const Ogre::Vector3& vel);
        const Ogre::Vector3& getPosition();
        void translate(const Ogre::Vector3& vec);
        void setPosition(const Ogre::Vector3& pos);
        virtual void setParentSceneNode(Ogre::SceneNode* sceneNode);
        virtual void onCollision(const CollisionEvent& evt);
        virtual ~GameObject() = 0;
        GameObject(Ogre::Entity* mesh);
    };
};

#endif
