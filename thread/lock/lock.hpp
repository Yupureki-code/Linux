#pragma once
#include <iostream>
#include <pthread.h>

#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

class mylock{
public:
    mylock()
    {
        int n = pthread_mutex_init(&_lock,nullptr);
        if(n != 0)
        {
            ERR_EXIT("mutex init");
            return;
        }
        //std::cout<<"mutex init success!"<<std::endl;
    }
    int lock()
    {
        return pthread_mutex_lock(&_lock);
    }
    int unlock()
    {
        return pthread_mutex_unlock(&_lock);
    }
    pthread_mutex_t* get_lock()
    {
        return &_lock;
    }
    ~mylock()
    {
        pthread_mutex_destroy(&_lock);
    }
private:
    pthread_mutex_t _lock;
};
