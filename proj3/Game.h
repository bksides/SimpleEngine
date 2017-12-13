#ifndef GAME_H
#define GAME_H

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreFrameListener.h>
#include <OISKeyboard.h>
#include "RaceWorld.h"

struct VehicleInfo {
	Ogre::Vector3 location;
	Ogre::Vector3 velocity;
	Ogre::Quaternion rotation;
	VehicleInfo(Ogre::Vector3 location,
		Ogre::Vector3 velocity,
		Ogre::Quaternion rotation) : location(location),
									velocity(velocity),
									rotation(rotation)
	{}
};

class Game
{
protected:
	Ogre::Camera*& mCamera;
	Ogre::Camera*& mTopCamera;
	Ogre::SceneManager*& mSceneMgr;
	bool& mShutDown;
public:
	Game(Ogre::Camera*& mCamera,
			Ogre::Camera*& mTopCamera,
			Ogre::SceneManager*& mSceneMgr,
			bool& mShutDown);
    virtual void createScene(void);
    virtual void createCamera(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased(const OIS::KeyEvent &arg );
    virtual ~Game() = 0;
    RaceWorld* raceWorld;
};

#endif