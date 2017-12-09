#include "NetworkProtocol.h"

/*
template <typename... Args>
void NetworkProtocol::addFunction<char*, Args>(std::string identifier, std::function<T(Args*...)> func)
{
    funcs[identifier] = [func](TCPsocket sock) {
        char* ret = func(read<Args>(sock)...);
        int size = strlen(ret);
        if(SDLNet_TCP_send(sock, &size, sizeof(size)) < sizeof(size))
        {
            return false;
        }
        if(SDLNet_TCP_send(sock, ret, size * sizeof(char)) < size * sizeof(char))
        {
            return false;
        }
        return true;
    };
}
*/

bool NetworkProtocol::call(int identifier, TCPsocket sock)
{
    if(funcs.count(identifier))
    {
        return funcs[identifier](sock);
    }
    return false;
}