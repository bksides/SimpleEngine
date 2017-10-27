#include "NetPaddle.h"

NetPaddle::NetPaddle(Ogre::SceneManager* mSceneMgr, TCPsocket socket) : Paddle(mSceneMgr)
{
	this->socket = socket;
}

void NetPaddle::onUpdate(float deltaTime)
{
	this->setPosition(mostRecentSentPosition);
}