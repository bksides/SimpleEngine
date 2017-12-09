#include "NetworkServer.h"
#include "NetworkProtocol.h"
#include <thread>
#include <iostream>

void threadloop(NetworkServer* server)
{
	if(server->sock == NULL)
	{
		std::cout << "\n\n\nTHE SOCKET WAS NOT CREATED\n\n\n";
	}

	SDLNet_SocketSet set;
	
	if(set = SDLNet_AllocSocketSet(1))
	{
		std::cout << "\n\n\nSuccessfully alloced socket set\n\n\n";
	}
	else
	{
		std::cout << "\n\n\nError on AllocSocketSet\n\n\n";
	}

	if(SDLNet_TCP_AddSocket(set, server->sock) == -1)
	{
		std::cout << "\n\n\nError on AddSocket.\n\n\n";
	}
	else
	{
		std::cout << "\n\n\nSuccessfully added socket\n\n\n";
	}

	while(!server->readyToTerminate())
	{
        SDLNet_CheckSockets(set, 0);
        if(SDLNet_SocketReady(server->sock))
		{
			std::cout << "GOT A CONNECTION\n";
			TCPsocket handlersock = SDLNet_TCP_Accept(server->sock);
			server->accept(handlersock);
			server->addClientSock(handlersock);
			std::thread* handlethread = new std::thread([server](TCPsocket sock) {
				while(true)
				{
					printf("Waiting for function call...\n");
					int identifier;
					if(SDLNet_TCP_Recv(sock, &identifier, sizeof(int)) <= 0)
					{
						std::cout << "\n\nERROR!!!\n\n" << SDLNet_GetError() << "\n\n";
						server->socketDisconnected(sock);
						break;
					}
					std::cout << "Received function call: " << identifier << "\n";
					server->protocol->call(identifier, sock);
					if(server->readyToTerminateClientSock(sock))
					{
						break;
					}
				}
			}, handlersock);
		}
	}
	std::cout << "\n\n\nTHREAD FINISHED EXECUTION\n\n\n";
}

void NetworkServer::addClientSock(TCPsocket sock)
{
	clientSockMutex.lock();
	clientsockets[sock] = false;
	clientSockMutex.unlock();
}

bool NetworkServer::readyToTerminateClientSock(TCPsocket sock)
{
	clientSockMutex.lock();
	bool ret = clientsockets[sock];
	clientSockMutex.unlock();
	return ret;
}

void NetworkServer::terminateClientSock(TCPsocket sock)
{
	clientSockMutex.lock();
	clientsockets[sock] = true;
	clientSockMutex.unlock();
}

NetworkServer::NetworkServer(int portnum,
	NetworkProtocol* protocol) : protocol(protocol)
{
	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, portnum);
	sock = SDLNet_TCP_Open(&ip);
}

void NetworkServer::go()
{
	netthread = new std::thread(threadloop, this);
}

bool NetworkServer::readyToTerminate()
{
	terminatemutex.lock();
	bool ret = _terminate;
	terminatemutex.unlock();
	return ret;
}

void NetworkServer::terminate()
{
	terminatemutex.lock();
	_terminate = true;
	terminatemutex.unlock();
}