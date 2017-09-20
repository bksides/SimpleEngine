#ifndef BALL_H
#define BALL_H

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "SimpleEngine/SimpleEngineGameObject.h"
#include "SimpleEngine/SimpleEngineCollisionEvent.h"
#include <SDL/SDL_mixer.h>

class Ball : public SimpleEngine::GameObject
{
public:
    float radius;
    static Mix_Chunk* boing;
    Ball(Ogre::SceneManager* mSceneMgr, float radius);
    void onCollision(const SimpleEngine::CollisionEvent& evt);
    void setParentSceneNode(Ogre::SceneNode* sceneNode);
};

#endif
