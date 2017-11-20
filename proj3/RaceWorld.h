#include "../SimpleEngine/SimpleEngineWorld.h"
#include "Vehicle.h"
#include <OgreSceneManager.h>

class RaceWorld : public SimpleEngine::World
{
public:
	Vehicle* playerVehicle = NULL;
	RaceWorld(Ogre::SceneManager* mSceneMgr);
};