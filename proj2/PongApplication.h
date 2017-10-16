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

extern int player_score;

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
    virtual void createCamera();
    virtual void createViewports();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    void CEGUI_Init(void);
};

#endif // #ifndef __PongApplication_h_
