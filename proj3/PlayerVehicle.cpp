#include "PlayerVehicle.h"

PlayerVehicle::PlayerVehicle(Ogre::SceneManager* mSceneMgr) : Vehicle(mSceneMgr) {}

void PlayerVehicle::setParentSceneNode(Ogre::SceneNode* node)
{
	Vehicle::setParentSceneNode(node);
	if(cameraNode)
	{
		this->node->addChild(cameraNode);
	}
	else
	{
		cameraNode = this->node->createChildSceneNode();
	}
}