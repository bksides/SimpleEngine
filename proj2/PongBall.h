#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>

class PongBall : public SimpleEngine::GameObject
{
public:
    PongBall(Ogre::SceneManager* mSceneMgr, btVector3 pos);
    void setParentSceneNode(Ogre::SceneNode* node);
};
