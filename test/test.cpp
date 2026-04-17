#include <jsoncpp/json/json.h>
#include <iostream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>

int main() 
{
    Json::Value json;
    json["x"] = 1;
    json["oper"] = '+';
    json["y"] = 2;
    Json::FastWriter w;
    std::string out = w.write(json);
    std::cout<<out;
    Json::Reader r;
    bool res = r.parse(out,json);
    if(res)
    {
        int x = json["x"].asInt();
        char oper = json["oper"].asInt();
        int y = json["y"].asInt();
        std::cout<<x<<" "<<oper<<" "<<y<<std::endl;
    }
    return 0;
}