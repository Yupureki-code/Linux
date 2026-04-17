#pragma once
#include "com.hpp"
#include "socket.hpp"

class Request
{
public:
    Request(int x = 0,char oper = ' ',int y = 0)
    :_x(x),_oper(oper),_y(y)
    {}

    void Serialize(std::string& out)
    {
        Json::Value json;
        json["x"] = _x;
        json["oper"] = _oper;
        json["y"] = _y;
        Json::FastWriter w;
        out = w.write(json);
    }

    bool Dserialize(std::string& out)
    {
        Json::Value json;
        Json::Reader r;
        bool res = r.parse(out,json);
        if(res)
        {
            _x = json["x"].asInt();
            _oper = json["oper"].asInt();
            _y = json["y"].asInt();
        }
        return res;
    }
    int get_x(){return _x;}
    char get_oper(){return _oper;}
    int get_y(){return _y;}
    std::string get_request_string()
    {
        return std::to_string(_x) + " " + _oper + " " + std::to_string(_y);
    }
private:
    int _x;
    char _oper;
    int _y;
};

class Reponse
{
public:
    Reponse(double ret = 0,int code = 0)
    :_result(ret),_code(code)
    {}
    Reponse(Reponse& rep)
    {
        _result = rep.get_ret();
        _code = rep.get_code();
    }
    Reponse(Reponse&& rep)
    {
        _result = rep.get_ret();
        _code = rep.get_code();
    }
    Reponse& operator=(Reponse&& rep)
    {
        _result = rep.get_ret();
        _code = rep.get_code();
        return *this;
    }
    Reponse& operator=(Reponse& rep)
    {
        _result = rep.get_ret();
        _code = rep.get_code();
        return *this;

    }
    void Serialize(std::string& out)
    {
        Json::Value json;
        json["result"] = _result;
        json["code"] = _code;
        Json::FastWriter w;
        out = w.write(json);
    }

    bool Dserialize(std::string& out)
    {
        Json::Value json;
        Json::Reader r;
        bool res = r.parse(out,json);
        if(res)
        {
            _result = json["result"].asDouble();
            _code = json["code"].asInt();
        }
        return res;
    }
    double get_ret()
    {
        return _result;
    }
    int get_code()
    {
        return _code;
    }
    std::string get_result_string()
    {
        return std::to_string(_result) + "[" +std::to_string(_code) + "]";
    }
private:
    double _result;
    int _code;
};

class Protocol
{
private:
    const std::string proto_sep = " ";
    const std::string line_sep = "\r\n";
    using func_t = std::function<Reponse(Request&)>;
public:
    Protocol()
    {}
    Protocol(func_t func)
    :_func(func)
    {}
    void Encode(std::string& message)
    {
        message = std::to_string(message.size()) + line_sep + message + line_sep; 
    }
    
    bool Decode(std::string& package,std::string& message)
    {
        int pos = package.find(line_sep);
        if(pos == std::string::npos)
            return false;
        std::string num = package.substr(0,pos);
        int total = std::stoi(num) + num.size() + 2 * line_sep.size();
        if(package.size() < total)
            return false;
        message = package.substr(pos + line_sep.size(),std::stoi(num));
        package.erase(0,total);
        return true;
    }

    bool get_request(std::shared_ptr<Socket>& sock,Request& req)
    {
        std::string package;
        while(1)
        {
            int n = sock->Recv(package);
            if(n > 0)
            {
                std::string message;
                if(!Decode(package,message))
                    continue;
                req.Dserialize(message);
                return true;
            }
            else if(n == 0)
                exit(ExitCode::NORMAL);
            else
                return false;
        }
    }
    bool get_reponse(std::shared_ptr<Socket>& sock,Reponse& rep)
    {
        std::string package;
        while(true)
        {
            int n = sock->Recv(package);
            if(n > 0)
            {
                std::string message;
                if(!Decode(package,message))
                    continue;
                rep.Dserialize(message);
                return true;
            }
            else if(n == 0)
                return false;
            else
                return false;
        }
    }
    Reponse handle(Request& req)
    {
        return _func(req);
    }
private:
    func_t _func;
};