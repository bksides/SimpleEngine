#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

class PongBall : public SimpleEngine::GameObject
{
public:
    PongBall(Ogre::SceneManager* mSceneMgr);
    void setParentSceneNode(Ogre::SceneNode* node);
};
