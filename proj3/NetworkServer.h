#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <list>
#include <SDL/SDL_net.h>
#include <functional>
#include <mutex>
#include <thread>
#include <map>

#include "NetworkProtocol.h"

class NetworkServer
{
private:
	std::thread* netthread;
	std::map<TCPsocket, bool> clientsockets;
	bool _terminate = false;
	std::mutex terminatemutex;
	std::mutex clientSockMutex;
public:
	NetworkProtocol* protocol;
	bool readyToTerminate();
	void terminate();
	void addClientSock(TCPsocket sock);
	bool readyToTerminateClientSock(TCPsocket sock);
	void terminateClientSock(TCPsocket sock);
	TCPsocket sock;
	std::function<void(TCPsocket)> accept;
	std::function<void(TCPsocket, NetworkServer*)> handle;
	NetworkServer(int portnum, NetworkProtocol* protocol);
	void go();
};

#endif
