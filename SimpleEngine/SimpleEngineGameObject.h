/*! \file SimpleEngineGameObject.h
    \brief File describing the SimpleEngine::GameObject class.
    */

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
    /*!
    \brief A class encapsulating objects within the game.

    This class contains information pertaining to objects
    such as meshes and rigidbodies.
    */
    class GameObject
    {
    protected:
        Ogre::Vector3 velocity = Ogre::Vector3::ZERO;
        /*!
        \brief The Ogre::SceneNode to which the mesh representing this object
        is attached.
        */
        Ogre::SceneNode* node = NULL;
        /*!
        \brief The Ogre::Entity mesh representing this object in the scene.
        */
        Ogre::Entity* mesh= NULL;
        /*!
        \brief The btRigidBody with which physics calculations are being performed
        for this GameObject
        */
        btRigidBody* rigidBody = NULL;
        /*!
        \brief A callback function that is called each update cycle.

        This function is declared virtual so that derived classes can define
        specific update behavior.

        \param deltaTime The time elapsed since the last update.
        */
        virtual void onUpdate(float deltaTime);
        GameObject();
    public:
        /*!
        \brief Returns a pointer to the btRigidBody representing this GameObject
        */
        btRigidBody* getRigidBody();
        /*!
        \brief A function that handles update logic for GameObjects.

        The onUpdate(deltaTime) function is called from here.

        \param deltaTime The time elapsed since the last update.
        */
        void update(float deltaTime);

        const Ogre::Vector3& getVelocity();
        void addVelocity(const Ogre::Vector3& vel);
        void setVelocity(const Ogre::Vector3& vel);

        /*!
        \brief Returns the current global position of this GameObject.
        */
        const Ogre::Vector3& getPosition();

        /*!
        \brief Adjusts the current global position of this GameObject.

        \param vec The Ogre::Vector3 representing the offset by which to translate
        this object.
        */
        void translate(const Ogre::Vector3& vec);

        /*!
        \brief Sets the current global position of this GameObject.

        \param pos The Ogre::Vector3 representing the new position of the object.
        */
        void setPosition(const Ogre::Vector3& pos);

        /*!
        \brief Returns a pointer to the Ogre::SceneNode to which this object's
        mesh is attached.
        */
        Ogre::SceneNode* getSceneNode();

        /*!
        \brief Creates a child node of sceneNode and attaches this GameObject's
        mesh to that node.

        \param sceneNode The node which is to become the parent of this GameObject's
        node
        */
        virtual void setParentSceneNode(Ogre::SceneNode* sceneNode);

        /*!
        \brief A callback function invoked whenever this GameObject collides with
        another GameObject.

        \param evt A CollisionEvent describing this collision.
        */
        virtual void onCollision(const CollisionEvent& evt);
        ~GameObject();

        /*!
        \param mesh The Ogre::Entity which should be used to render this GameObject.

        \param rb The btRigidBody which should be used to perform physics calculations
        for this GameObject.
        */
        GameObject(Ogre::Entity* mesh, btRigidBody* rb = NULL);
    };
};

#endif
