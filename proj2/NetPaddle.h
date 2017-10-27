#include "Paddle.h"
#include <OgreSceneManager.h>
#include <SDL/SDL_net.h>

class NetPaddle : public Paddle
{
private:
	TCPsocket socket;
	Ogre::Vector3 mostRecentSentPosition = Ogre::Vector3::UNIT_Z*49;
public:
	NetPaddle(Ogre::SceneManager* mSceneMgr, TCPsocket socket);
	virtual void onUpdate(float deltaTime);
};