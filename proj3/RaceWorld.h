#include "../SimpleEngine/SimpleEngineWorld.h"
#include "PlayerVehicle.h"
#include <OgreSceneManager.h>

class RaceWorld : public SimpleEngine::World
{
public:
	PlayerVehicle* playerVehicle = NULL;
	Ogre::Vector3 cameraAngle = Ogre::Vector3::UNIT_Z;
	RaceWorld(Ogre::SceneManager* mSceneMgr);
};