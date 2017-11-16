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

class RaceApplication : public BaseApplication
{
public:
    RaceApplication(void);
    virtual ~RaceApplication(void);
protected:
    virtual void createScene(void);
    virtual void createCamera();
    virtual void createViewports();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
    RaceWorld* raceWorld;
};

#endif // #ifndef __RaceApplication_h_
