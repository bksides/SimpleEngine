#include "Game.h"

Game::Game(Ogre::Camera*& mCamera, Ogre::SceneManager*& mSceneMgr,
			bool& mShutDown) : mCamera(mCamera), mSceneMgr(mSceneMgr),
			mShutDown(mShutDown) {}

void Game::createScene(void) {}

void Game::createCamera(void) {}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	return true;
}


bool Game::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

bool Game::keyReleased(const OIS::KeyEvent &arg )
{
	return true;
}

Game::~Game() {}