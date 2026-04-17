#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "com.hpp"
#include <memory>

class Socket
{
public:
    Socket()
    {}
    ~Socket()
    {}
    virtual void create_socket() = 0;
    virtual void Bind(uint16_t) = 0;
    virtual void Listen(int) = 0;
    virtual std::shared_ptr<Socket> Accept(InetAddr&) = 0;
    virtual bool Connect(const std::string&,const uint16_t&) = 0;
    virtual int Recv(std::string&) = 0;
    virtual void Send(const std::string&) = 0;
    virtual int get_sockfd() = 0;
    void InitTcpServer(uint16_t port = DEFAULT_PORT,int backlog = DEFAULT_BACKLOG)
    {
        create_socket();
        Bind(port);
        Listen(backlog);
    }
    void InitTcpClient(std::string ip = DEFAULT_IP,uint16_t port = DEFAULT_PORT)
    {
        create_socket();
        Connect(ip,port);
    }
};

class TcpSocket : public Socket
{
private:
    using func_t = std::function<void()>;
public:
    TcpSocket(int sockfd = DEFAULT_SOCKFD)
    :_sockfd(sockfd)
    {}
    void create_socket() override
    {
        _sockfd = socket(AF_INET,SOCK_STREAM,0);
        
        if(_sockfd < 0)
            exit(ExitCode::SOCKET);
    }
    void Bind(uint16_t port)override
    {
        InetAddr addr(port);
        int n = bind(_sockfd,CONV(addr.get_addr()),sizeof(addr.get_addr()));
        if(n < 0)
            exit(ExitCode::BIND);
    }
    void Listen(int backlog)override
    {
        int n = listen(_sockfd,backlog);
        if(n < 0)
            exit(ExitCode::LISTEN);
    }
    std::shared_ptr<Socket> Accept(InetAddr& addr)override
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int sockfd = accept(_sockfd,CONV(peer),&len);
        if(sockfd < 0)
            return nullptr;
        InetAddr tmp(peer);
        addr = tmp;
        std::shared_ptr<Socket> p = std::make_shared<TcpSocket>(sockfd);
        return p;
    }
    bool Connect(const std::string& ip,const uint16_t& port)override
    {
        InetAddr addr(ip,port);
        int n = connect(_sockfd,CONV(addr.get_addr()),sizeof(addr.get_addr()));
        if(n < 0)
            return false;
        return true;
    }
    int Recv(std::string& out)override
    {
        char buffer[MAXNUM];
        ssize_t n = recv(_sockfd,buffer,sizeof(buffer) - 1,0);
        if(n > 0)
        {
            buffer[n] = '\0';
            out += buffer;
            return n;
        }
        else if(n == 0)
            return 0;
        else
            return -1;
    }
    void Send(const std::string& out)override
    {
        send(_sockfd,out.c_str(),sizeof(out),0);
    }
    int get_sockfd()override
    {
        return _sockfd;
    }
private:
    int _sockfd;
};