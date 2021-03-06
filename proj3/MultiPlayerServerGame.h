#ifndef MULTI_PLAYER_SERVER_GAME_H
#define MULTI_PLAYER_SERVER_GAME_H

#include <list>
#include <SDL/SDL_net.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreFrameListener.h>
#include <OISKeyboard.h>

#include "Game.h"
#include "Vehicle.h"
#include "RaceWorld.h"
#include "RaceApplication.h"

class MultiPlayerServerGame : public Game
{
private:
	std::map<TCPsocket, Vehicle*> remotePlayers;
	std::set<OIS::KeyCode> pressedKeys;
    unsigned int clientSeed;
    RaceApplication* app;
public:
	MultiPlayerServerGame(Ogre::Camera*& mCamera,
			Ogre::Camera*& mTopCamera,
			Ogre::SceneManager*& mSceneMgr,
			bool& mShutDown, std::list<TCPsocket> clients,
			unsigned int clientSeed,
			RaceApplication* app);
    void createScene(void);
    void createCamera(void);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased(const OIS::KeyEvent &arg );
	~MultiPlayerServerGame();
};

#endif