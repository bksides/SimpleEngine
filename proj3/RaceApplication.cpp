#include "PlayerVehicle.h"
#include "RaceApplication.h"
#include "FloorTile.h"
#include "StartTile.h"
#include "TrackCreator.h"
#include "Wall.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <OgreMeshManager.h>
#include <OgreQuaternion.h>
#include <OgreMath.h>
#include <OISKeyboard.h>

//-------------------------------------------------------------------------------------
RaceApplication::RaceApplication(void)
{
    
}
//-------------------------------------------------------------------------------------
RaceApplication::~RaceApplication(void)
{
    delete raceWorld;
}

void RaceApplication::CEGUI_Init()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

/*    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);*/

/*  Use this if you want a cool demo of what CEGUI can do with sheets
    CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TextDemo.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);
*/
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    pause_pop_up = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/PausePopUp");

    //score_board->setProperty("HorzFormatting","HorzCentred");
    pause_pop_up->setText("You Win!");
    pause_pop_up->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(.25, 0)));
    pause_pop_up->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.5, 0)));
    pause_pop_up->setVisible(false);

    sheet->addChild(pause_pop_up);
    
    //mult_menu->setSize(CEGUI::USize(CEGUI::UDim(1.0,0.0), CEGUI::UDim(1.0, 0.0)));
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

//-------------------------------------------------------------------------------------
void RaceApplication::createScene(void)
{

    CEGUI_Init();

    /**
    ---SKYBOX---
    **/

    mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

    /**
    ---LIGHTING---
    **/

    //Create room light
    Ogre::Light* lamp = mSceneMgr->createLight("lamp");
    lamp->setType(Ogre::Light::LT_DIRECTIONAL);
    lamp->setDirection(-40,-50,40);
    lamp->setDiffuseColour(1,1,1);
    lamp->setSpecularColour(1,1,1);
    lamp->setAttenuation(200, 0, 0, .0002);

    //Create ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.6,0.6,0.6));

    //Set shadow rendering mode
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    raceWorld = new RaceWorld(mSceneMgr);
    raceWorld->playerVehicle = new PlayerVehicle(mSceneMgr);
    raceWorld->addObject(raceWorld->playerVehicle, Ogre::Vector3::UNIT_Y*10);
    raceWorld->playerVehicle->cameraNode->attachObject(mCamera);

    TrackCreator tc;
    std::list<DIRECTION::DIRECTION> turns = tc.createTrack();
    int x = 0;
    int z = 0;
    bool start = true;
    DIRECTION::DIRECTION curdir = DIRECTION::LEFT;
    //DIRECTION::DIRECTION turn = turns.front();
    for(DIRECTION::DIRECTION turn : turns)
    {
        if(start)
        {
            raceWorld->addObject(new StartTile(mSceneMgr), Ogre::Vector3(x*100, 0, z*100), Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
            raceWorld->trackcoords.insert(coord{.x = x, .y = z});
            start = false;
        }
        else
        {
            raceWorld->addObject(new FloorTile(mSceneMgr), Ogre::Vector3(x*100, 0, z*100), Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);
            raceWorld->trackcoords.insert(coord{.x = x, .y = z});
        }
        if(curdir != DIRECTION::RIGHT && turn != DIRECTION::LEFT)
        {
            raceWorld->addObject(new Wall(mSceneMgr), Ogre::Vector3(x*100-50, 0, z*100), Ogre::Vector3::ZERO, Ogre::Vector3(0, M_PI/2, M_PI/-2));
        }
        if(curdir != DIRECTION::DOWN && turn != DIRECTION::UP)
        {
            raceWorld->addObject(new Wall(mSceneMgr), Ogre::Vector3(x*100, 0, z*100+50), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI/-2, 0, 0));
        }
        if(curdir != DIRECTION::LEFT && turn != DIRECTION::RIGHT)
        {
            raceWorld->addObject(new Wall(mSceneMgr), Ogre::Vector3(x*100+50, 0, z*100), Ogre::Vector3::ZERO, Ogre::Vector3(0, M_PI/2, M_PI/2));
        }
        if(curdir != DIRECTION::UP && turn != DIRECTION::DOWN)
        {
            raceWorld->addObject(new Wall(mSceneMgr), Ogre::Vector3(x*100, 0, z*100-50), Ogre::Vector3::ZERO, Ogre::Vector3(M_PI/2, 0, 0));
        }
        curdir = turn;
        switch(turn)
        {
            case DIRECTION::LEFT:
                x -= 1;
                break;
            case DIRECTION::UP:
                z += 1;
                break;
            case DIRECTION::RIGHT:
                x += 1;
                break;
            case DIRECTION::DOWN:
                z -= 1;
                break;
        }
    }
}

//--------------------------------------------------------------------------------------
bool RaceApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    mCamera->lookAt(raceWorld->playerVehicle->getPosition());
    if (pressedKeys.find(OIS::KC_W) != pressedKeys.end())
    {
        raceWorld->playerVehicle->getRigidBody()->activate(true);
        raceWorld->playerVehicle->addVelocity(Ogre::Vector3(mCamera->getDerivedDirection().x, 0, mCamera->getDerivedDirection().z)*evt.timeSinceLastFrame*30);
        if(raceWorld->playerVehicle->getVelocity().length() > 150)
        {
            raceWorld->playerVehicle->setVelocity(raceWorld->playerVehicle->getVelocity()*150/raceWorld->playerVehicle->getVelocity().length());
        }
    }
    if (pressedKeys.find(OIS::KC_S) != pressedKeys.end())
    {
        raceWorld->playerVehicle->getRigidBody()->activate(true);
        raceWorld->playerVehicle->addVelocity(-1*Ogre::Vector3(mCamera->getDerivedDirection().x, 0, mCamera->getDerivedDirection().z).normalisedCopy()*evt.timeSinceLastFrame*30);
        if(raceWorld->playerVehicle->getVelocity().length() > 150)
        {
            raceWorld->playerVehicle->setVelocity(raceWorld->playerVehicle->getVelocity()*150/raceWorld->playerVehicle->getVelocity().length());
        }
    }
    if (pressedKeys.find(OIS::KC_A) != pressedKeys.end())
    {
        raceWorld->playerVehicle->cameraNode->yaw(Ogre::Radian(M_PI * evt.timeSinceLastFrame), Ogre::Node::TS_WORLD);
    }
    if (pressedKeys.find(OIS::KC_D) != pressedKeys.end())
    {
        raceWorld->playerVehicle->cameraNode->yaw(Ogre::Radian(-1*M_PI * evt.timeSinceLastFrame), Ogre::Node::TS_WORLD);
    }
    raceWorld->playerVehicle->visitedTiles.insert(coord{(int)(floor((float)(raceWorld->playerVehicle->getPosition().x+50)/100)), (int)(floor((float)(raceWorld->playerVehicle->getPosition().z+50)/100))});
    if((int)(floor((float)(raceWorld->playerVehicle->getPosition().x+50)/100)) == 0 && (int)(floor((float)(raceWorld->playerVehicle->getPosition().z+50)/100)) == 0)
    {
        if(raceWorld->playerVehicle->visitedTiles == raceWorld->trackcoords)
        {
            pause_pop_up->setVisible(true);
        }
    }
    raceWorld->update(evt.timeSinceLastFrame);
    return BaseApplication::frameRenderingQueued(evt);
}

//--------------------------------------------------------------------------------------
void RaceApplication::createCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(0,40,100);
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);
}

//--------------------------------------------------------------------------------------
void RaceApplication::createViewports()
{
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0.3));
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) /
        Ogre::Real(vp->getActualHeight()));
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        RaceApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
