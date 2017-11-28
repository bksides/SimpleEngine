#include "Tile.h"

Tile::Tile(Ogre::SceneManager mSceneMgr, TileType type)
{
    Ogre::Plane floorPlane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "floor",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        floorPlane,
        100,100,20,20,
        true,
        1,5,5,
        Ogre::Vector3::UNIT_Y);

    int i = 0;

    meshes[i] = mSceneMgr->createEntity("floor");
    mesh->setMaterialName("Examples/Rockwall");
    ++i;

    if(type == TileType::STRAIGHT || type == TileType::RIGHT)
    {
    	Ogre::Plane leftWallPlane(Ogre::Vector3::UNIT_X, 0);
    	Ogre::MeshManager::getSingleton().createPlane(
        	"leftWall",
        	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	leftWallPlane,
        	10,10,20,20,
        	true,
        	1,5,5,
        	Ogre::Vector3::UNIT_X);
    	meshes[i] = mSceneMgr->createEntity("leftWall");
    	mesh->setMaterialName("Examples/Rockwall");
    	++i;
    }
    if(type == TileType::STRAIGHT || type == TileType::LEFT)
    {
    	Ogre::Plane rightWallPlane(Ogre::Vector3::NEGATIVE_UNIT_X, 0);
    	Ogre::MeshManager::getSingleton().createPlane(
        	"rightWall",
        	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	rightWallPlane,
        	10,10,20,20,
        	true,
        	1,5,5,
        	Ogre::Vector3::NEGATIVE_UNIT_X);
    	meshes[i] = mSceneMgr->createEntity("rightWall");
    	mesh->setMaterialName("Examples/Rockwall");
    	++i;
    }
    if(type == TileType::RIGHT || type == TileType::LEFT)
    {
    	Ogre::Plane backWallPlane(Ogre::Vector3::UNIT_Z, 0);
    	Ogre::MeshManager::getSingleton().createPlane(
        	"backWall",
        	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	backWallPlane,
        	10,10,20,20,
        	true,
        	1,5,5,
        	Ogre::Vector3::UNIT_Z);
    	meshes[i] = mSceneMgr->createEntity("backWall");
    	mesh->setMaterialName("Examples/Rockwall");
    	++i;
    }
}