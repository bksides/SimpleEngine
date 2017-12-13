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
#include "Game.h"
#include "NetworkServer.h"

#include <SDL/SDL_mixer.h>
#include <SDL/SDL_net.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

enum WinStates
{
    WON,
    LOST,
    INCOMPLETE
};

class RaceApplication : public BaseApplication
{
public:
    RaceApplication(void);
    virtual ~RaceApplication(void);
    CEGUI::Window* pause_pop_up = NULL;
    CEGUI::Window* start_menu = NULL;
    CEGUI::Window *mult_menu = NULL;
    CEGUI::Window* mult_info = NULL;
    CEGUI::Window* join_menu = NULL;
    CEGUI::PushButton* startGame = NULL;
    CEGUI::RadioButton* hostOption = NULL;
    unsigned int clientSeed;
    NetworkServer* server = NULL;
    bool startgame = false;
    bool startmultgame = false;
    bool startclientgame = false;
    bool stopAsking = false;
    bool winnerFound = false;
    bool winnerIsMe = false;
    TCPsocket winner = NULL;
    WinStates winState = WinStates::INCOMPLETE;
    std::mutex startmultgamemutex;
    std::list<struct VehicleInfo*> vehicleList;
    std::map<Vehicle*, struct VehicleInfo*> vehicles;
    std::map<TCPsocket, Vehicle*> playerVehicles;
    std::map<Vehicle*, bool> finished;
    CEGUI::Editbox* toConnect;
    CEGUI::OgreRenderer* mRenderer;
    void createStartMenu(CEGUI::WindowManager& wmgr);
    CEGUI::Window* player_slots[16];
    std::map<TCPsocket, int> socket_to_player_slot;
    void beginMultiPlayerAsClient(void);
    Game* game = NULL;
protected:
    void CEGUI_Init();
    virtual void createScene(void);
    virtual void createCamera();
    virtual void createViewports();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    void createMultiPlayerMenu(CEGUI::WindowManager& wmgr);
    void showMultiPlayerOptions(void);
    void backToMenu(void);
    void createJoinMenu(CEGUI::WindowManager& wmgr);
    void showJoinMenu(void);
    void beginMultiPlayer(void);
    void serverLobbyMode();
    //void clientLobbyMode();
};

#endif // #ifndef __RaceApplication_h_
