#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "../SimpleEngine/SimpleEngineGameObject.h"

class Vehicle : public SimpleEngine::GameObject
{
public:
	Vehicle(Ogre::SceneManager* mSceneMgr);
	void setParentSceneNode(Ogre::SceneNode* node);
};