#ifndef SINGLE_PLAYER_GAME_H
#define SINGLE_PLAYER_GAME_H

#include "Game.h"
#include "RaceWorld.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <OISKeyboard.h>
#include <ctime>

class SinglePlayerGame : public Game
{
public:
	SinglePlayerGame(Ogre::Camera*& mCamera,
			Ogre::Camera*& mTopCamera,
			Ogre::SceneManager*& mSceneMgr,
			bool& mShutDown,
			CEGUI::Window*& pause_pop_up,
			CEGUI::Window*& score_board);
	void createScene(void);
	void createCamera(void);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased(const OIS::KeyEvent &arg );
	~SinglePlayerGame();
private:
    CEGUI::Window* pause_pop_up;
    CEGUI::Window* score_board;
	std::set<OIS::KeyCode>	pressedKeys;
	bool lapTimerStarted = false;
	std::clock_t start;
	unsigned bestTime = 0;
};

#endif