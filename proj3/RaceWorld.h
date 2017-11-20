#include "../SimpleEngine/SimpleEngineWorld.h"
#include "Vehicle.h"
#include <OgreSceneManager.h>

class RaceWorld : public SimpleEngine::World
{
public:
	Vehicle* playerVehicle = NULL;
	cameraAngle = Ogre::Vector3::UNIT_Z;
	RaceWorld(Ogre::SceneManager* mSceneMgr);
};