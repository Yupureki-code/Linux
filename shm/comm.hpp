#pragma one

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#define PATHNAME "."
#define ID 0x6666
#define CREATER "creater"
#define USER "user"
#define MAXNUM 4096

#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

class Shm
{
public:
    Shm(std::string path = PATHNAME, int id = ID, std::string user = USER)
    {
        _key = ftok(path.c_str(), id);
        if (_key == -1)
        {
            ERR_EXIT("ftok");
            exit(1);
        }
        _user = user;
        if (_user == CREATER)
            create(IPC_CREAT | IPC_EXCL | 0666);
        else
            create(IPC_CREAT | 0666);
    }

    bool attach()
    {
        mem = (char *)shmat(_shm_id, NULL, 0);
        if (mem == (void *)-1)
        {
            ERR_EXIT("shmat");
            exit(1);
        }
        std::cout << "attach success" << std::endl;
        return true;
    }

    bool detach()
    {
        if (shmdt(mem) == -1)
        {
            ERR_EXIT("shmdt");
            exit(1);
        }
        if (_user == CREATER)
        {
            int n = shmctl(_shm_id, IPC_RMID, NULL);
            if (n < 0)
            {
                ERR_EXIT("shmctl");
                exit(1);
            }
            std::cout << "detach success" << std::endl;
            return true;
        }
        return false;
    }

    int get_key()
    {
        return _key;
    }

    int get_shm_id()
    {
        return _shm_id;
    }

    ~Shm()
    {
        detach();
    }

    char *get_mem()
    {
        return mem;
    }

private:
    void create(int flag)
    {
        _shm_id = shmget(_key, MAXNUM, flag);
        if (_shm_id == -1)
        {
            ERR_EXIT("shmget");
            exit(1);
        }
    }
    key_t _key;
    int _shm_id;
    std::string _user;
    char *mem;
};