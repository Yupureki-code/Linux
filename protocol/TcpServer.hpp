#pragma once

#include "socket.hpp"

class TcpServer : public nocopy
{
private:
    using func_t = std::function<void(std::shared_ptr<Socket>&,InetAddr&)>;
public:
    TcpServer(uint16_t port,func_t func)
    :_port(port),_ioserver(func),_listensock(std::make_shared<TcpSocket>())
    {}
    void init(int backlog = DEFAULT_BACKLOG)
    {
        _listensock->InitTcpServer(_port,backlog);
        _listensockfd = _listensock->get_sockfd();
    }
    void run()
    {
        _isrunning = true;
        while(_isrunning)
        {
            InetAddr addr;
            std::shared_ptr<Socket> sock = _listensock->Accept(addr);
            if(sock == nullptr)
                continue;
            pid_t pid = fork();
            if(pid > 0)
            {
                close(sock->get_sockfd());
                waitpid(pid,nullptr,0);
            }
            else if(pid == 0)
            {
                if(fork() > 0)
                    exit(ExitCode::NORMAL);
                close(_listensock->get_sockfd());
                _ioserver(sock,addr);
            }
            else
                exit(ExitCode::FORK);
        }
    }

private:
    uint16_t _port;
    int _listensockfd;
    bool _isrunning = false;
    std::shared_ptr<Socket> _listensock;
    func_t _ioserver;
};