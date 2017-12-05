#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <list>
#include <SDL/SDL_net.h>
#include <functional>
#include <mutex>
#include <thread>

class NetworkServer
{
private:
	std::thread* netthread;
	bool _terminate = false;
	std::mutex terminatemutex;
public:
	bool readyToTerminate();
	void terminate();
	TCPsocket sock;
	std::function<void(TCPsocket)> accept;
	NetworkServer(int portnum, std::function<void(TCPsocket)> accept = [](TCPsocket sock){});
	void go();
};

#endif