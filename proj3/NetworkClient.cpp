#include "NetworkClient.h"

NetworkClient::NetworkClient(std::string hostname, int portnum)
{
	IPaddress ip;
	SDLNet_ResolveHost(&ip, hostname.c_str(), portnum);
	sock = SDLNet_TCP_Open(&ip);
}