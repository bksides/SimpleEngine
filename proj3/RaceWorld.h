#ifndef RACE_WORLD_H
#define RACE_WORLD_H

#include "../SimpleEngine/SimpleEngineWorld.h"
#include "PlayerVehicle.h"
#include <OgreSceneManager.h>
#include <unordered_set>

class RaceWorld : public SimpleEngine::World
{
public:
	std::unordered_set<struct coord> trackcoords;
	PlayerVehicle* playerVehicle = NULL;
	Ogre::Vector3 cameraAngle = Ogre::Vector3::UNIT_Z;
	RaceWorld(Ogre::SceneManager* mSceneMgr);
};

#endif