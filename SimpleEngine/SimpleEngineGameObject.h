#ifndef SIMPLE_ENGINE_GAME_OBJECT_H
#define SIMPLE_ENGINE_GAME_OBJECT_H

#include <OgreSceneNode.h>
#include <OgreVector3.h>
#include <OgreEntity.h>
#include <OgreMaterial.h>
#include "SimpleEngineCollisionEvent.h"
#include <btBulletDynamicsCommon.h>

namespace SimpleEngine
{
    class GameObject
    {
    protected:
        Ogre::Vector3 velocity = Ogre::Vector3::ZERO;
        Ogre::SceneNode* node = NULL;
        Ogre::Entity* mesh= NULL;
        btRigidBody* rigidBody = NULL;
        virtual void onUpdate(float deltaTime);
    public:
        btRigidBody* getRigidBody();
        void update(float deltaTime);
        const Ogre::Vector3& getVelocity();
        void addVelocity(const Ogre::Vector3& vel);
        void setVelocity(const Ogre::Vector3& vel);
        const Ogre::Vector3& getPosition();
        void translate(const Ogre::Vector3& vec);
        void setPosition(const Ogre::Vector3& pos);
        Ogre::SceneNode* getSceneNode();
        virtual void setParentSceneNode(Ogre::SceneNode* sceneNode);
        virtual void onCollision(const CollisionEvent& evt);
        ~GameObject();
        GameObject(Ogre::Entity* mesh, btRigidBody* rb = NULL);
    };
};

#endif
