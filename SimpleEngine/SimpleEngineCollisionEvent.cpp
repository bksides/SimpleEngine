#include <OgreVector3.h>
#include "SimpleEngineGameObject.h"

SimpleEngine::CollisionEvent::CollisionEvent(SimpleEngine::GameObject* obj,
    Ogre::Vector3 vec) : collider(obj), normal(vec)
{
}
