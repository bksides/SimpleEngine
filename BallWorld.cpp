#include "BallWorld.h"
#include "Ball.h"
#include "SimpleEngine/SimpleEngineGameObject.h"
#include <OgreVector3.h>
#include <OgreSceneManager.h>

void BallWorld::onUpdate(float deltaTime)
{
    for(SimpleEngine::GameObject* obj : objects)
    {
        obj->addVelocity(gravity*deltaTime);

        if(abs(obj->getPosition().x) > size-((Ball*)obj)->radius)
        {
            SimpleEngine::CollisionEvent evt(nullptr,
                Ogre::Vector3(obj->getPosition().x, 0, 0).normalisedCopy());
            obj->onCollision(evt);
            obj->setVelocity(obj->getVelocity() - 2*obj->getVelocity().x*Ogre::Vector3::UNIT_X);
        }
        if(abs(obj->getPosition().y) > size-((Ball*)obj)->radius)
        {
            SimpleEngine::CollisionEvent evt(nullptr,
                Ogre::Vector3(0, obj->getPosition().y, 0).normalisedCopy());
            obj->onCollision(evt);
            obj->setVelocity(obj->getVelocity() - 2*obj->getVelocity().y*Ogre::Vector3::UNIT_Y);
        }
        if(abs(obj->getPosition().z) > size-((Ball*)obj)->radius)
        {
            SimpleEngine::CollisionEvent evt(nullptr,
                Ogre::Vector3(obj->getPosition().z, 0, 0).normalisedCopy());
            obj->onCollision(evt);
            obj->setVelocity(obj->getVelocity() - 2*obj->getVelocity().z*Ogre::Vector3::UNIT_Z);
        }
    }
}

BallWorld::BallWorld(float size, Ogre::Vector3 gravity,
    Ogre::SceneManager* mSceneMgr) : SimpleEngine::World(mSceneMgr), size(size), gravity(gravity) {}
