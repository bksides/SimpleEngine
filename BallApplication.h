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
#ifndef __BallApplication_h_
#define __BallApplication_h_

#include "BaseApplication.h"
#include "BallWorld.h"

#include <SDL/SDL_mixer.h>

class BallApplication : public BaseApplication
{
public:
    BallApplication(void);
    virtual ~BallApplication(void);
protected:
    virtual void createScene(void);
    virtual void createCamera();
    virtual void createViewports();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
    Mix_Chunk* boing;
    std::map<Ogre::SceneNode*, Ogre::Vector3> spheres;
    BallWorld* ballWorld;
};

#endif // #ifndef __BallApplication_h_
