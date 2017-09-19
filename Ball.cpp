#include "SimpleEngine/SimpleEngineGameObject.h"
#include "Ball.h"
#include "SimpleEngine/SimpleEngineCollisionEvent.h"
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <SDL/SDL_mixer.h>

Mix_Chunk* Ball::boing = Mix_LoadWAV( "../media/sounds/boing.wav" );

void Ball::onCollision(const SimpleEngine::CollisionEvent& evt)
{
    if(abs(velocity.z) > 3)
    {
        Mix_PlayChannel( -1, boing, 0 );
    }
}

Ball::Ball(Ogre::SceneManager* mSceneMgr, float radius) :
    SimpleEngine::GameObject(
        mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE)), radius(radius)
{
    mesh->setMaterialName("Ball/Skin");
}

void Ball::setParentSceneNode(Ogre::SceneNode* sceneNode)
{
    GameObject::setParentSceneNode(sceneNode);
    node->scale(radius*0.01, radius*0.01, radius*0.01);
}
