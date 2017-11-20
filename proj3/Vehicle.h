#ifndef VEHICLE_H
#define VEHICLE_H

#include <OgreSceneManager.h>
#include "../SimpleEngine/SimpleEngineGameObject.h"

class Vehicle : public SimpleEngine::GameObject
{
public:
	Vehicle(Ogre::SceneManager* mSceneMgr);
};
#endif VEHICLE_H