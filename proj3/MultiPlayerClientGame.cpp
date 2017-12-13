#include "MultiPlayerClientGame.h"
#include <btBulletDynamicsCommon.h>
#include "StartTile.h"
#include "FloorTile.h"
#include "Wall.h"

MultiPlayerClientGame::MultiPlayerClientGame(Ogre::Camera*& mCamera,
            Ogre::Camera*& mTopCamera,
			Ogre::SceneManager*& mSceneMgr,
			bool& mShutDown,
			unsigned int seed, RaceApplication* app) : Game(mCamera, mTopCamera, mSceneMgr, mShutDown), seed(seed),
            app(app)
{
	for(struct VehicleInfo* info : app->vehicleList)
	{
        Vehicle* veh = new Vehicle(mSceneMgr);
		app->vehicles[veh] = info;
        veh->getRigidBody()->setCollisionFlags(veh->getRigidBody()->getCollisionFlags());
	}
}

void MultiPlayerClientGame::createScene(void)
{
	/**
    ---SKYBOX---
    **/

    mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

    /**
    ---LIGHTING---
    **/

    //Create room light
    Ogre::Light* lamp = mSceneMgr->createLight("lamp");
    lamp->setType(Ogre::Light::LT_DIRECTIONAL);
    lamp->setDirection(-40,-50,40);
    lamp->setDiffuseColour(1,1,1);
    lamp->setSpecularColour(1,1,1);
    lamp->setAttenuation(200, 0, 0, .0002);

    //Create ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.6,0.6,0.6));

    //Set shadow rendering mode
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    raceWorld = new RaceWorld(mSceneMgr);
    raceWorld->playerVehicle = new PlayerVehicle(mSceneMgr);
    raceWorld->addObject(raceWorld->playerVehicle, Ogre::Vector3::UNIT_Y*10);
    raceWorld->playerVehicle->cameraNode->attachObject(mCamera);
    raceWorld->playerVehicle->getSceneNode()->attachObject(mTopCamera);
    mTopCamera->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(-M_PI/3));

    for(std::pair<Vehicle*, struct VehicleInfo*> mappair : app->vehicles)
    {
    	struct VehicleInfo* info = mappair.second;
    	raceWorld->addObject(mappair.first, info->location, info->velocity, Ogre::Vector3::ZERO);
        mappair.first->setRotation(info->rotation);
    }

    TrackCreator tc;
    std::list<DIRECTION::DIRECTION> turns = tc.createTrack(25, 25, seed);
    int x = 0;
    int z = 0;
    bool start = true;
    DIRECTION::DIRECTION curdir = DIRECTION::LEFT;
    //DIRECTION::DIRECTION turn = turns.front();
    for(DIRECTION::DIRECTION turn : turns)
    {
        if(start)
        {
            raceWorld->addObject(new StartTile(mSceneMgr), Ogre::Vector3(x*100, 0, z*100), Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
            raceWorld->trackcoords.insert(coord{.x = x, .y = z});
            start = false;
        }
        else
        {
            raceWorld->addObject(new FloorTile(mSceneMgr), Ogre::Vector3(x*100, 0, z*100), Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
            raceWorld->trackcoords.insert(coord{.x = x, .y = z});
        }
        if(curdir != DIRECTION::RIGHT && turn != DIRECTION::LEFT)
        {
            raceWorld->addObject(new Wall(mSceneMgr), Ogre::Vector3(x*100-50, 0, z*100), Ogre::Vector3::ZERO, Ogre::Vector3(0, M_PI/2, M_PI/-2));
        }
        if(curdir != DIRECTION::DOWN && turn != DIRECTION::UP)
        {
            raceWorld->addObject(new Wall(mSceneMgr), Ogre::Vector3(x*100, 0, z*100+50), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI/-2, 0, 0));
        }
        if(curdir != DIRECTION::LEFT && turn != DIRECTION::RIGHT)
        {
            raceWorld->addObject(new Wall(mSceneMgr), Ogre::Vector3(x*100+50, 0, z*100), Ogre::Vector3::ZERO, Ogre::Vector3(0, M_PI/2, M_PI/2));
        }
        if(curdir != DIRECTION::UP && turn != DIRECTION::DOWN)
        {
            raceWorld->addObject(new Wall(mSceneMgr), Ogre::Vector3(x*100, 0, z*100-50), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI/2, 0, 0));
        }
        curdir = turn;
        switch(turn)
        {
            case DIRECTION::LEFT:
                x -= 1;
                break;
            case DIRECTION::UP:
                z += 1;
                break;
            case DIRECTION::RIGHT:
                x += 1;
                break;
            case DIRECTION::DOWN:
                z -= 1;
                break;
        }
    }
}

void MultiPlayerClientGame::createCamera(void)
{
    //std::cout << "\n\n\n\n\nCREATING CAMERA\n\n\n\n\n";
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(0,40,100);
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);
}

bool MultiPlayerClientGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    mCamera->lookAt(raceWorld->playerVehicle->getPosition());
    if (pressedKeys.find(OIS::KC_W) != pressedKeys.end())
    {
        raceWorld->playerVehicle->getRigidBody()->activate(true);
        raceWorld->playerVehicle->addVelocity(Ogre::Vector3(mCamera->getDerivedDirection().x, 0, mCamera->getDerivedDirection().z)*evt.timeSinceLastFrame*30);
        if(raceWorld->playerVehicle->getVelocity().length() > 150)
        {
            raceWorld->playerVehicle->setVelocity(raceWorld->playerVehicle->getVelocity()*150/raceWorld->playerVehicle->getVelocity().length());
        }
    }
    if (pressedKeys.find(OIS::KC_S) != pressedKeys.end())
    {
        raceWorld->playerVehicle->getRigidBody()->activate(true);
        raceWorld->playerVehicle->addVelocity(-1*Ogre::Vector3(mCamera->getDerivedDirection().x, 0, mCamera->getDerivedDirection().z).normalisedCopy()*evt.timeSinceLastFrame*30);
        if(raceWorld->playerVehicle->getVelocity().length() > 150)
        {
            raceWorld->playerVehicle->setVelocity(raceWorld->playerVehicle->getVelocity()*150/raceWorld->playerVehicle->getVelocity().length());
        }
    }
    if (pressedKeys.find(OIS::KC_A) != pressedKeys.end())
    {
        raceWorld->playerVehicle->cameraNode->yaw(Ogre::Radian(M_PI * evt.timeSinceLastFrame), Ogre::Node::TS_WORLD);
    }
    if (pressedKeys.find(OIS::KC_D) != pressedKeys.end())
    {
        raceWorld->playerVehicle->cameraNode->yaw(Ogre::Radian(-1*M_PI * evt.timeSinceLastFrame), Ogre::Node::TS_WORLD);
    }
    raceWorld->playerVehicle->visitedTiles.insert(coord{(int)(floor((float)(raceWorld->playerVehicle->getPosition().x+50)/100)), (int)(floor((float)(raceWorld->playerVehicle->getPosition().z+50)/100))});
    /*
    if((int)(floor((float)(raceWorld->playerVehicle->getPosition().x+50)/100)) == 0 && (int)(floor((float)(raceWorld->playerVehicle->getPosition().z+50)/100)) == 0)
    {
        if(raceWorld->playerVehicle->visitedTiles == raceWorld->trackcoords)
        {
            pause_pop_up->setVisible(true);
        }
    }
    */

    for(std::pair<Vehicle*, struct VehicleInfo*> mappair : app->vehicles)
    {
        mappair.first->setPosition(mappair.second->location);
        mappair.first->setVelocity(mappair.second->velocity);
        mappair.first->setRotation(mappair.second->rotation);
    }

    raceWorld->update(evt.timeSinceLastFrame);
    return true;
}

bool MultiPlayerClientGame::keyPressed( const OIS::KeyEvent &arg )
{
  if (arg.key == OIS::KC_ESCAPE) {
    mShutDown = true;
  }
  else
  {
	pressedKeys.insert(arg.key);
  }

  return true;
}

bool MultiPlayerClientGame::keyReleased(const OIS::KeyEvent &arg)
{
	pressedKeys.erase(arg.key);

    return true;
}

MultiPlayerClientGame::~MultiPlayerClientGame()
{
	delete raceWorld;
}