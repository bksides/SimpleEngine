#ifndef BALL_WORLD_H
#define BALL_WORLD_H

#include <OgreVector3.h>
#include "../SimpleEngine/SimpleEngineWorld.h"
#include <OgreSceneManager.h>

class BallWorld : public SimpleEngine::World
{
protected:
    float size;
    Ogre::Vector3 gravity;
    void onUpdate(float deltaTime);
public:
    BallWorld(float size, Ogre::Vector3 gravity,
        Ogre::SceneManager* mSceneMgr);
};

#endif
