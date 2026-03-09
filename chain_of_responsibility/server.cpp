#include "MsgQueue.hpp"
#include "ChainOfResponsibility.hpp"

int main()
{
    Server server;
    HandlerTextEntry handler;
    while(1)
    {
        std::string buffer;
        buffer = server.Recv(CLIENT_TYPE);
        if(buffer == "exit")
            break;
        std::cout<<"client say:"<<buffer<<std::endl;
        handler.Run(buffer);
    }
    return 0;
}