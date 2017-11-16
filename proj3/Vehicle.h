#include <OgreSceneManager.h>
#include "../SimpleEngine/SimpleEngineGameObject.h"

class Vehicle : public SimpleEngine::GameObject
{
public:
	Vehicle(Ogre::SceneManager* mSceneMgr);
};