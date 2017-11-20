#ifndef VEHICLE_H
#define VEHICLE_H

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "../SimpleEngine/SimpleEngineGameObject.h"

class Vehicle : public SimpleEngine::GameObject
{
public:
	Vehicle(Ogre::SceneManager* mSceneMgr);
	void setParentSceneNode(Ogre::SceneNode* node);
};
#endif VEHICLE_H