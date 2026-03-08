#include "MsgQueue.hpp"

int main()
{
    Server server;
    while(1)
    {
        std::string buffer;
        buffer = server.Recv(CLIENT_TYPE);
        std::cout<<"client say:"<<buffer<<std::endl;
    }
    return 0;
}