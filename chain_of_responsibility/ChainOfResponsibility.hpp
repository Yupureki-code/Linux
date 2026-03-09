#pragma once

#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <memory>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <sys/types.h>
#include <sys/wait.h>

class HandlerText
{
public:
    virtual void Execute(std::string& str) = 0;
    void Set_next(std::shared_ptr<HandlerText>& next)
    {
        _next = next;
    }
    void Enable()
    {
        _is_enable = true;
    }
    void Unable()
    {
        _is_enable = false;
    }
protected:
    bool _is_enable = true;
    std::shared_ptr<HandlerText> _next;
};

class HandlerTextFormat : public HandlerText
{
public:
    void Execute(std::string& str)override
    {
        if(_is_enable)
        {
            str = std::to_string(time(nullptr)) + "-" + std::to_string(getpid()) + "-" + str + "\n";
            std::cout<<"Format:" <<str<<std::endl;
        }
        if(_next)
        {
            _next->Execute(str);
        }
        else
        {
            std::cout<<"end chain"<<std::endl;
        }
    }
private:
};

#define DEFAULT_PATH "./tmp/"
#define DEFAULT_FILENAME "log.txt"

class HandlerTextSaveFile : public HandlerText
{
public:
    HandlerTextSaveFile(std::string path = DEFAULT_PATH,std::string filename = DEFAULT_FILENAME)
    :_path(path),_filename(filename)
    {
        std::string _path_file = _path + _filename;
        _fd = open(_path_file.c_str(),O_WRONLY | O_CREAT | O_APPEND,0644);
        if(_fd < 0)
        {
            std::cout<<"open file error"<<std::endl;
            Unable();
            return;
        }
    }
    void Execute(std::string& str)override
    {
        if(_is_enable)
        {
            int n = write(_fd,str.c_str(),str.size());
            if(n < 0)
            {
                std::cout<<"write error"<<std::endl;
                return;
            }
            std::cout<<"SaveFile "<<_path + _filename<<":"<<str<<std::endl;
        }
        if(_next)
        {
            _next->Execute(str);
        }
        else
        {
            std::cout<<"end chain"<<std::endl;
        }
    }
private:
    std::string _path;
    std::string _filename;
    int _fd;
};

#define MAX_LINE 5

class HandlerTextBackUp : public HandlerText
{
public:
    HandlerTextBackUp(std::string path = DEFAULT_PATH,std::string filename = DEFAULT_FILENAME,int max_line = MAX_LINE)
    :_ifs(path + filename),_path(path),_filename(filename),_max_line(max_line)
    {
        if(!_ifs.is_open())
        {
            std::cout<<"open file error"<<std::endl;
            Unable();
            return;
        }
    }
    void Execute(std::string& str)override
    {
        if(_is_enable)
        {
            int line = 0;
            std::string buffer;
            if (!_ifs.is_open())
                return;
            while(std::getline(_ifs,buffer))
            {
                std::cout<<buffer<<std::endl;
                line++;
            }
            if(line > MAX_LINE)
                BackUp();
        }
        if(_next)
        {
            _next->Execute(str);
        }
        else
        {
            std::cout<<"end chain"<<std::endl;
        }
    }
    void BackUp()
    {
        std::string newfile = _filename + "-" + std::to_string(time(nullptr));
        if(fork() == 0)
        {
            chdir(_path.c_str());
            rename(_filename.c_str(),newfile.c_str());
            newfile += ".zip";
            std::cout<<"remove "<<newfile<<" and zip it";
            execlp("zip","zip","-r",newfile.c_str(),"./",NULL);
            std::cout<<"execute error!"<<std::endl;
            exit(1);
        }
        waitpid(-1,nullptr,0);
        std::string tmp = _path + newfile;
        remove(tmp.c_str());
    }
private:
    std::string _path;
    std::string _filename;
    std::ifstream _ifs;
    int _max_line;
};

class HandlerTextEntry
{
public:
    HandlerTextEntry()
    {
        _format = std::make_shared<HandlerTextFormat>();
        _savefile = std::make_shared<HandlerTextSaveFile>();
        _backup = std::make_shared<HandlerTextBackUp>();
        _format->Set_next(_savefile);
        _savefile->Set_next(_backup);
    }
    void Run(std::string& str)
    {
        if(_format)
            _format->Execute(str);
    }
private:
    std::shared_ptr<HandlerText> _format;
    std::shared_ptr<HandlerText> _savefile;
    std::shared_ptr<HandlerText> _backup;

};