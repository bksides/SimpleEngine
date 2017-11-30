#ifndef VEHICLE_H
#define VEHICLE_H

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <unordered_set>
#include "TrackCreator.h"

class Vehicle : public SimpleEngine::GameObject
{
public:
	std::unordered_set<struct coord> visitedTiles;
public:
	Vehicle(Ogre::SceneManager* mSceneMgr);
	void setParentSceneNode(Ogre::SceneNode* node);
};
#endif //VEHICLE_H