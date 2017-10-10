#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>

class Wall : public SimpleEngine::GameObject
{
public:
    Wall(Ogre::SceneManager* mSceneMgr);
};
