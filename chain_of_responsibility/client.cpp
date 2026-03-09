#include "MsgQueue.hpp"

int main()
{
    Client client;
    while(1)
    {
        std::cout<<"input->";
        std::string buffer;std::cin>>buffer;
        client.Send(CLIENT_TYPE,buffer);
    }
    return 0;
}