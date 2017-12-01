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
#ifndef __RaceApplication_h_
#define __RaceApplication_h_

#include "BaseApplication.h"
#include "RaceWorld.h"

#include <SDL/SDL_mixer.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

class RaceApplication : public BaseApplication
{
public:
    RaceApplication(void);
    virtual ~RaceApplication(void);
    CEGUI::Window* pause_pop_up = NULL;
    CEGUI::OgreRenderer* mRenderer;
protected:
    void CEGUI_Init();
    virtual void createScene(void);
    virtual void createCamera();
    virtual void createViewports();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
    RaceWorld* raceWorld;
};

#endif // #ifndef __RaceApplication_h_
