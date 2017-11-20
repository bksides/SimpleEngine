#ifndef PLAYER_VEHICLE_H
#define PLAYER_VEHICLE_H

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "Vehicle.h"

class PlayerVehicle : public Vehicle
{
public:
	PlayerVehicle(Ogre::SceneManager* mSceneMgr);
	Ogre::SceneNode* cameraNode = NULL;
	void setParentSceneNode(Ogre::SceneNode* node);
};

#endif