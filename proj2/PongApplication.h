/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __PongApplication_h_
#define __PongApplication_h_

#include "BaseApplication.h"
#include "../SimpleEngine/SimpleEngineWorld.h"
#include "../SimpleEngine/SimpleEngineGameObject.h"
#include <SDL/SDL_net.h>
#include <thread>

extern int player_score;
extern SimpleEngine::GameObject* paddle;
extern SimpleEngine::GameObject* ball;
extern Ogre::Vector3 ballMostRecentSentPosition;
extern bool client;
extern bool terminating;
extern std::thread* netthread;

class PongApplication : public BaseApplication
{
public:
    PongApplication(void);
    virtual ~PongApplication(void);
    void updateScoreboard(void);
protected:
    CEGUI::OgreRenderer* mRenderer;
    SimpleEngine::World* wallWorld;
    virtual void createScene(void);
    virtual void createMultiPlayerScene(TCPsocket socket);
    virtual void createCamera();
    virtual void createViewports();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    void beginGame(void);
    void CEGUI_Init(void);
    void createStartMenu(CEGUI::WindowManager& wmgr);
    void createScoreBoard(CEGUI::WindowManager& wmgr);
    void createPauseMenu(CEGUI::WindowManager& wmgr);
    void createMultiPlayerMenu(CEGUI::WindowManager& wmgr);
    void backToMenu(void);
    void showMultiPlayerOptions(void);
    void beginSinglePlayer(void);
    void beginMultiPlayer(void);
    void prepareMultiPlayer(void);
};

#endif // #ifndef __PongApplication_h_
