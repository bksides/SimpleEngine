#ifndef GAME_H
#define GAME_H

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreFrameListener.h>
#include <OISKeyboard.h>

class Game
{
protected:
	Ogre::Camera*& mCamera;
	Ogre::SceneManager*& mSceneMgr;
	bool& mShutDown;
public:
	Game(Ogre::Camera*& mCamera,
			Ogre::SceneManager*& mSceneMgr,
			bool& mShutDown);
    virtual void createScene(void);
    virtual void createCamera(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased(const OIS::KeyEvent &arg );
    virtual ~Game() = 0;
};

#endif