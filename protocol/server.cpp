#include "NetCal.hpp"
#include "protocol.hpp"
#include "TcpServer.hpp"
#include "com.hpp"

#define DEBUG

int main(int argv, char *argc[])
{
    if (argv != 2)
        exit(ExitCode::FORMAT);
    std::shared_ptr<cal> netcal = std::make_shared<cal>();
    std::shared_ptr<Protocol> prot = std::make_shared<Protocol>([&netcal](Request &req) -> Reponse
                                                                { return netcal->func(req); });
    TcpServer server(atoi(argc[1]), [&prot](std::shared_ptr<Socket> &server, InetAddr &addr)
    {
        while(1)
        {
            Request req;
            prot->get_request(server,req);
            std::cout<<"request : "<<req.get_request_string()<<std::endl;
            Reponse rep = prot->handle(req);
            std::cout<<"reponse : "<<rep.get_result_string()<<std::endl;
            std::string message;
            rep.Serialize(message);
            prot->Encode(message);
            server->Send(message);
        } 
    }
    );
    server.init();
    server.run();
}