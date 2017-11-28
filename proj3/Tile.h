#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <OgreSceneNode.h>
#include <OgreEntity.h>

enum TileType
{
	STRAIGHT,
	RIGHT,
	LEFT
}

class Tile : public SimpleEngine::GameObject
{
public:
	Ogre::Entity* meshes[3];
	StraightTile(Ogre::SceneManager mSceneMgr, TileType type);
	void setParentSceneNode(Ogre::SceneNode* node);
}