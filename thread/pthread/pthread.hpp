#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <memory>
#include "../lock/lock.hpp"

int count = 1;
#define ERR_EXIT(m)         \
    do                      \
    {                       \
        perror(m);          \
        exit(EXIT_FAILURE); \
    } while (0)

struct thread_info
{
    pthread_t _tid;
    std::string _name;
    bool _is_detach;
    int _code;
    void *_args;
    thread_info(std::string name, bool is_detach, void *args)
        : _name(name), _is_detach(is_detach), _args(args), _code(count++)
    {
    }
    void exit()
    {
        pthread_exit(nullptr);
    }
};

class mythread
{
public:
    mythread(std::string name, bool is_detach, void *(*thread_run)(void *), void *args)
    {
        thread_node = new thread_info(name, is_detach, args);
        int n = pthread_create(&thread_node->_tid, nullptr, thread_run, (void *)(thread_node));
        if (is_detach)
            pthread_detach(thread_node->_tid);
        if (n != 0)
        {
            ERR_EXIT("phread_create");
            return;
        }
        // else
        //     std::cout << "pthread: " << name << " create success!" << std::endl;
    }
    void change_name(std::string name)
    {
        thread_node->_name = name;
    }
    std::string get_name()
    {
        return thread_node->_name;
    }
    int get_code()
    {
        return thread_node->_code;
    }
    pthread_t get_tid()
    {
        return thread_node->_tid;
    }
    bool is_detach()
    {
        return thread_node->_is_detach;
    }
    void *join()
    {
        int n = pthread_join(thread_node->_tid, &_ret);
        if (n != 0)
            ERR_EXIT("join");
        delete thread_node;
        return _ret;
    }
    bool kill()
    {
        int n = pthread_cancel(thread_node->_tid);
        if (n != 0)
        {
            ERR_EXIT("kill");
            return false;
        }
        delete thread_node;
        return true;
    }
    void *get_args()
    {
        return thread_node->_args;
    }

private:
    thread_info* thread_node;
    void *_ret;
};