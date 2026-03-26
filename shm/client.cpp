#include "comm.hpp"

int main()
{
    Shm shm(PATHNAME,ID,USER);
    std::cout<<"shm id: "<<shm.get_shm_id()<<std::endl;
    shm.attach();
    char *mem = shm.get_mem();
    while(true)
    {
        fgets(mem,MAXNUM,stdin);
        mem[strlen(mem) - 1] = '\0';
        if(strcmp(mem,"exit") == 0) 
            break;
        std::cout<<"client input: "<<mem<<std::endl;
    }
    return 0;
}