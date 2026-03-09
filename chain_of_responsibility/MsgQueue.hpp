#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>

#define MSG_CREATE IPC_CREAT | IPC_EXCL | 0666
#define MSG_GET IPC_CREAT

#define DEFAULT_PATHNAME "/tmp"
#define DEFAULT_ID 'A'
#define DEFAULT_SIZE 1024

#define CLIENT_TYPE 1
#define SERVER_TYPE 2

class MsgQueue
{
private:
    struct msgbuf {
        long mtype;
        char mtext[DEFAULT_SIZE];
    };
public:
    MsgQueue(std::string pathname = DEFAULT_PATHNAME,int id = DEFAULT_ID)
    {
        _key = ftok(pathname.c_str(),id);
        if(_key < 0)
        {
            std::cout<<"ftok error"<<std::endl;
            return;
        }
        std::cout<<"key:"<<_key<<std::endl;
    }
    void Create(int flag)
    {
        _msqid = msgget(_key,flag);
        if(_msqid < 0)
        {
            std::cout<<"msgget error"<<std::endl;
            return;
        }
        std::cout<<"msqid:"<<_msqid<<std::endl;
    }
    void Send(int type,const std::string& buffer)
    {
        msgbuf tmp;
        tmp.mtype = type;
        buffer.copy(tmp.mtext,sizeof(tmp),0);
        int n = msgsnd(_msqid,&tmp,buffer.size(),0);
        if(n < 0)
        {
            std::cout<<"msgsnd error"<<std::endl;
            return;
        }
    }
    std::string Recv(int type)
    {
        msgbuf tmp;
        memset(&tmp,0,sizeof(tmp));
        int n = msgrcv(_msqid,&tmp,sizeof(tmp.mtext),type,0);
        if(n >= 0)
        {
            tmp.mtext[n] = '\0';
            return tmp.mtext;
        }
    }
    ~MsgQueue()
    {
        if(_msqid)
            msgctl(_msqid,IPC_RMID,nullptr);
    }
private:
    int _msqid = -1;
    key_t _key;
};

class Server : public MsgQueue
{
public:
    Server()
    {
        Create(MSG_CREATE);
    }
private:
};

class Client : public MsgQueue
{
public:
    Client()
    {
        Create(MSG_GET);
    }
private:
};