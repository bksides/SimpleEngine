#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>

class Paddle : public SimpleEngine::GameObject
{
public:
	Paddle(Ogre::SceneManager* mSceneManager);
};