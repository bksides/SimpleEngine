#include "NetPaddle.h"
#include "PongApplication.h"
#include <SDL/SDL_net.h>
#include <thread>
#include <unistd.h>

enum DATA_TYPE {
	Ball_Location,
	Paddle_Location,
	Player_Score,
	Opponent_Score
};

struct netdata {
	DATA_TYPE dt;
	union {
		Ogre::Vector3 vec;
		int score;
	};
};

void updateRemotePosition(NetPaddle* netpaddle)
{
	usleep(100000);
	struct netdata data = {Paddle_Location, paddle->getPosition()};
	SDLNet_TCP_Send(netpaddle->socket, &data, sizeof(netdata));
	SDLNet_SocketSet sockcheck = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(sockcheck, netpaddle->socket);

	while(SDLNet_CheckSockets(sockcheck, -1) > 0)
	{
		if(terminating)
		{
			std::terminate();
		}
		do
		{
			SDLNet_TCP_Recv(netpaddle->socket, &data, sizeof(netdata));
			switch(data.dt)
			{
				case Ball_Location:
					ballMostRecentSentPosition = data.vec;
					break;
				case Paddle_Location:
					netpaddle->mostRecentSentPosition = Ogre::Vector3(-1*data.vec.x, data.vec.y, -1*data.vec.z);
					break;
				case Player_Score:
					player_score = data.score;
					break;
				case Opponent_Score:
					opponent_score = data.score;
					break;
			}
		}
		while(SDLNet_CheckSockets(sockcheck, 0) > 0);

		data.dt = Paddle_Location;
		data.vec = paddle->getPosition();

		SDLNet_TCP_Send(netpaddle->socket, &data, sizeof(netdata));
		if(!client)
		{
			data.dt = Ball_Location;
			data.vec = ball->getPosition();
			SDLNet_TCP_Send(netpaddle->socket, &data, sizeof(netdata));
			data.dt = Player_Score;
			data.score = opponent_score;
			SDLNet_TCP_Send(netpaddle->socket, &data, sizeof(netdata));
			data.dt = Opponent_Score;
			data.score = player_score;
			SDLNet_TCP_Send(netpaddle->socket, &data, sizeof(netdata));
		}
	}
}

NetPaddle::NetPaddle(Ogre::SceneManager* mSceneMgr, TCPsocket socket) : Paddle(mSceneMgr)
{
	this->socket = socket;
	netthread = new std::thread(updateRemotePosition, this);
}

void NetPaddle::onUpdate(float deltaTime)
{
	this->setPosition(mostRecentSentPosition);
}