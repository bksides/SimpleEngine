#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>

class FloorTile : public SimpleEngine::GameObject
{
public:
    FloorTile(Ogre::SceneManager* mSceneMgr);
};
