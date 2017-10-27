#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneManager.h>

#ifndef PADDLE_H
#define PADDLE_H

class Paddle : public SimpleEngine::GameObject
{
public:
	Paddle(Ogre::SceneManager* mSceneManager);
};

#endif