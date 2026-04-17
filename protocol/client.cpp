#include "NetCal.hpp"
#include "protocol.hpp"
#include "socket.hpp"
#include "com.hpp"

int x,y;char oper;

void input()
{
    std::cout<<"enter x ->";
    std::cin>>x;
    std::cout<<"enter y ->";
    std::cin>>y;
    std::cout<<"enter oper ->";
    std::cin>>oper;
}

int main(int argv,char* argc[])
{
    if(argv != 3)
    {
        logger(LogLevel::INFO)<<"format: ./"<<__FILE__<<" ip port";
        exit(ExitCode::FORMAT);
    }
    std::shared_ptr<Socket> sock = std::make_shared<TcpSocket>();
    sock->InitTcpClient(argc[1],atoi(argc[2]));
    std::shared_ptr<Protocol> prot = std::make_shared<Protocol>();
    while(1)
    {
        input();
        Request req(x,oper,y);
        std::cout<<"your request: "<<req.get_request_string()<<std::endl;
        std::string out;
        req.Serialize(out);
        prot->Encode(out);
        sock->Send(out);
        Reponse rep;
        prot->get_reponse(sock,rep);
        std::cout<<"result: "<<rep.get_result_string()<<std::endl;
    }
    return 0;
}