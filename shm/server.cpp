#include "comm.hpp"

int main()
{
    Shm shm(PATHNAME,ID,CREATER);
    std::cout<<"shm id: "<<shm.get_shm_id()<<std::endl;
    shm.attach();
    char * mem = shm.get_mem();
    while(true)
    {
        if(strcmp(mem,"exit") == 0)
            break;
        std::cout<<"client input: "<<mem<<std::endl;
        sleep(1);
    }
    return 0;
}