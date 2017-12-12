#ifndef MULTI_PLAYER_CLIENT_GAME_H
#define MULTI_PLAYER_CLIENT_GAME_H

#include "Game.h"
#include "RaceWorld.h"
#include "Vehicle.h"
#include "RaceApplication.h"

class MultiPlayerClientGame : public Game
{
private:
	std::set<OIS::KeyCode> pressedKeys;
	std::vector<Vehicle*> otherPenguins;
	unsigned int seed;
	RaceApplication* app;
public:
	MultiPlayerClientGame(Ogre::Camera*& mCamera,
			Ogre::SceneManager*& mSceneMgr,
			bool& mShutDown,
			unsigned int seed,
			RaceApplication* app);
	std::map<Vehicle*, struct VehicleInfo*> vehicles;
    void createScene(void);
    void createCamera(void);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased(const OIS::KeyEvent &arg );
	~MultiPlayerClientGame();
};

#endif