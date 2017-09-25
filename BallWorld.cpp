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
			obj->setPosition(Ogre::Vector3((obj->getPosition().x > 0 ? 1 : -1) * (size - ((Ball*)obj)->radius), obj->getPosition().y, obj->getPosition().z));
            SimpleEngine::CollisionEvent evt(NULL,
                Ogre::Vector3(obj->getPosition().x, 0, 0).normalisedCopy());
            obj->onCollision(evt);
            obj->setVelocity(obj->getVelocity() - 2*obj->getVelocity().x*Ogre::Vector3::UNIT_X);
        }
        if(abs(obj->getPosition().y) > size-((Ball*)obj)->radius)
        {
			obj->setPosition(Ogre::Vector3(obj->getPosition().x, (obj->getPosition().y > 0 ? 1 : -1) * (size - ((Ball*)obj)->radius), obj->getPosition().z));
            SimpleEngine::CollisionEvent evt(NULL,
                Ogre::Vector3(0, obj->getPosition().y, 0).normalisedCopy());
            obj->onCollision(evt);
            obj->setVelocity(obj->getVelocity() - 2*obj->getVelocity().y*Ogre::Vector3::UNIT_Y);
        }
        if(abs(obj->getPosition().z) > size-((Ball*)obj)->radius)
        {
			obj->setPosition(Ogre::Vector3(obj->getPosition().x, obj->getPosition().y, (obj->getPosition().z > 0 ? 1 : -1) * (size - ((Ball*)obj)->radius)));
            SimpleEngine::CollisionEvent evt(NULL,
                Ogre::Vector3(obj->getPosition().z, 0, 0).normalisedCopy());
            obj->onCollision(evt);
            obj->setVelocity(obj->getVelocity() - 2*obj->getVelocity().z*Ogre::Vector3::UNIT_Z);
        }
		for(SimpleEngine::GameObject* obj2 : objects)
		{
			Ogre::Vector3 collisionVector = obj->getPosition() - obj2->getPosition();
			if(collisionVector.length() < ((Ball*)obj)->radius + ((Ball*)obj2)->radius && obj != obj2)
			{
				//Move ball outside radius of other ball
				Ogre::Vector3 postColPosition = obj2->getPosition()+collisionVector.normalisedCopy()*(((Ball*)obj)->radius+((Ball*)obj2)->radius);
				obj->setPosition(postColPosition);

				//reflect ball velocity along contact normal
				Ogre::Vector3 velocityProjection1 = (obj->getVelocity().dotProduct(collisionVector)/collisionVector.dotProduct(collisionVector))*collisionVector;
				Ogre::Vector3 velocityProjection2 = (obj2->getVelocity().dotProduct(collisionVector)/collisionVector.dotProduct(collisionVector))*collisionVector;
				obj->addVelocity(-1*velocityProjection1 + velocityProjection2);
				obj2->addVelocity(-1*velocityProjection2 + velocityProjection1);
			}
		}
    }
}

BallWorld::BallWorld(float size, Ogre::Vector3 gravity,
    Ogre::SceneManager* mSceneMgr) : SimpleEngine::World(mSceneMgr), size(size), gravity(gravity) {}
