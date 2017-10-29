#include "NetPaddle.h"
#include "PongApplication.h"
#include <SDL/SDL_net.h>
#include <thread>

struct posdata {
	bool isForBall;
	Ogre::Vector3 vector;
};

void updateRemotePosition(NetPaddle* netpaddle)
{
	struct posdata newPos1;
	struct posdata newPos2;
	struct posdata paddlePos;
	paddlePos.isForBall = false;
	paddlePos.vector = paddle->getPosition();
	SDLNet_TCP_Send(netpaddle->socket, &paddlePos, sizeof(struct posdata));
	while(SDLNet_TCP_Recv(netpaddle->socket, &newPos1, sizeof(struct posdata)) > 0)
	{
		if(newPos1.isForBall)
		{
			ballMostRecentSentPosition = newPos1.vector;
		}
		else
		{
			netpaddle->mostRecentSentPosition = newPos1.vector+99*Ogre::Vector3::UNIT_Z;
		}

		if(client)
		{
			SDLNet_TCP_Recv(netpaddle->socket, &newPos2, sizeof(struct posdata));
			if(newPos2.isForBall)
			{
				ballMostRecentSentPosition = -1*(newPos2.vector);
			}
			else
			{
				netpaddle->mostRecentSentPosition = newPos2.vector+99*Ogre::Vector3::UNIT_Z;
			}
		}

		paddlePos.isForBall = false;
		paddlePos.vector = paddle->getPosition();
		SDLNet_TCP_Send(netpaddle->socket, &paddlePos, sizeof(struct posdata));
		if(!client)
		{
			struct posdata ballPos;
			ballPos.isForBall = true;
			ballPos.vector = ball->getPosition();
			SDLNet_TCP_Send(netpaddle->socket, &ballPos, sizeof(struct posdata));
		}
	}
}

NetPaddle::NetPaddle(Ogre::SceneManager* mSceneMgr, TCPsocket socket) : Paddle(mSceneMgr)
{
	this->socket = socket;
	new std::thread(updateRemotePosition, this);
}

void NetPaddle::onUpdate(float deltaTime)
{
	this->setPosition(mostRecentSentPosition);
}