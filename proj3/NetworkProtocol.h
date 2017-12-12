#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include <functional>
#include <map>
#include <string>
#include <utility>
#include <SDL/SDL_net.h>

class NetworkProtocol
{
private:
    std::map< int, std::function<bool(TCPsocket)> > funcs;
public:
	template <typename T>
	static T* read(TCPsocket sock)
	{
		T* ret = (T*)malloc(sizeof(T));
		if(SDLNet_TCP_Recv(sock, (void*)ret, sizeof(T)) <= 0)
		{
			return NULL;
		}
		return ret;
	}

	template <typename T, typename... Args>
	void addFunction(int identifier, std::function<std::pair<T*, int>(TCPsocket, Args*...)> func)
	{
	    funcs[identifier] = [func](TCPsocket sock) {
	        std::pair<T*, int> result = func(sock, read<Args>(sock)...);
	        int size = result.second;
	        T* ret = result.first;
	        if(SDLNet_TCP_Send(sock, &size, sizeof(size)) < sizeof(size))
	        {
	            return false;
	        }
	        if(SDLNet_TCP_Send(sock, ret, size * sizeof(T)) < size * sizeof(T))
	        {
	            return false;
	        }
	        return true;
	    };
	}

    bool call(int identifier, TCPsocket sock);
};

#endif