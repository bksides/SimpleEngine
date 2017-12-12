#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <SDL/SDL_net.h>
#include <string>
#include <iostream>

class NetworkClient
{
private:
	TCPsocket sock;
public:
	struct Sendable
	{
		void* obj;
		int size;
	};
	template<typename ...Args>
	bool send(Sendable arg)
	{
		if(arg.obj == NULL)
		{
			return true;
		}
		return SDLNet_TCP_Send(sock, arg.obj, arg.size) > 0;
	}
	template <typename T, typename ...Args>
	std::pair<T*, int> call(int identifier, Args*... args)
	{
		if(SDLNet_TCP_Send(sock, &identifier, sizeof(int)) < sizeof(int))
		{
			return std::pair<T*, int>(NULL, 0);
		}
		Sendable arguments[] = {{(void*)args, sizeof(Args)}...};
		for(Sendable arg : arguments)
		{
			if(!send(arg))
			{
				return std::pair<T*, int>(NULL, 0);
			}
		}
		int len;
		if(SDLNet_TCP_Recv(sock, &len, sizeof(int)) <= 0)
		{
			return std::pair<T*, int>(NULL, 0);
		}
		T* ret = (T*)calloc(len + 1, sizeof(T));
		if(SDLNet_TCP_Recv(sock, ret, len * sizeof(T)) <= 0)
		{
			std::cout << "ERROR: did not receive all the data expected.\n";
			std::cout << "Expected " << len << "objects.\n";
			return std::pair<T*, int>(NULL, 0);
		}
		return std::pair<T*, int>(ret, len);
	}
	NetworkClient(std::string hostname, int portnum);
};

#endif