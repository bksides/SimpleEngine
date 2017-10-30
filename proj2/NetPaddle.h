#include "Paddle.h"
#include <OgreSceneManager.h>
#include <SDL/SDL_net.h>
#include <thread>

class NetPaddle : public Paddle
{
private:
	std::thread updatethread;
public:
	Ogre::Vector3 mostRecentSentPosition = Ogre::Vector3::UNIT_Z*49;
	TCPsocket socket;
	NetPaddle(Ogre::SceneManager* mSceneMgr, TCPsocket socket);
	virtual void onUpdate(float deltaTime);
};