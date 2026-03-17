#include <cstdint>
#include <mysql/mysql.h>
#include <iostream>
#include <string>

const std::string default_host = "127.0.0.1";
const std::string default_user = "Yu";
const std::string password = "lijiayu071024"; 
const std::string db = "test";
const std::uint16_t default_port = 3306;

int main()
{
    MYSQL* myself = mysql_init(nullptr);
    if(myself == nullptr)
    {
        std::cout<<"mysql init error"<<std::endl;
        return 1;
    }
    if(!mysql_real_connect(myself,default_host.c_str(),default_user.c_str(),password.c_str(),db.c_str(),default_port,nullptr,0))
    {
        std::cout<<"connect mysql error: "<<mysql_error(myself)<<std::endl;
        mysql_close(myself);
        return 1;
    }
    std::cout<<"connect mysql success"<<std::endl;
    std::string command = "select * from test";
    int n = mysql_query(myself,command.c_str());
    if(n == 0)
        std::cout<<"mysql query success"<<std::endl;
    else
        std::cout<<"mysql query error: "<<mysql_error(myself)<<std::endl;
    MYSQL_RES* res = mysql_store_result(myself);
    if(res == nullptr)
    {
        std::cout<<"mysql store result error"<<std::endl;
        return 1;
    }
    int i = 0;
    MYSQL_ROW line;
    for(; i < mysql_num_rows(res); i++)
    {
        auto line = mysql_fetch_row(res);
        int j = 0;
        for(; j < mysql_num_fields(res); j++)
        {
            std::cout<<line[j]<<" ";
        }
        std::cout<<std::endl;
    }
    mysql_close(myself);
    return 0;
}