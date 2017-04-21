#ifndef _JSON_PARSE_HPP_
#define _JSON_PARSE_HPP_
#include <json/json.h>
#include "structure.hpp"
#include <fstream>

#include <string.h>
#include "wrap.hpp"
using namespace std;

class JsonParse
{
public:

    void get_listen_arg(ListenArg &arg)
    {
        conf.open("config.json", ios::binary);
        if(reader.parse(conf, root))
        {
            memset(&arg, 0, sizeof arg);
            strcpy(arg.listen_ip, root["listen"]["ip"].asCString());
            arg.listen_port = root["listen"]["port"].asInt();
        }
        conf.close();
    }

    void get_memcached_arg(MemcachedArg &arg)
    {
        conf.open("config.json", ios::binary);
        if(reader.parse(conf, root))
        {  
            memset(&arg, 0, sizeof arg);
            strcpy(arg.memcached_ip, root["memcached"]["ip"].asCString());
            arg.memcached_port = root["memcached"]["port"].asInt();
        }
        conf.close();
    }

    void get_mysql_arg(MysqlArg &arg)
    {
        conf.open("config.json", ios::binary);
        if(reader.parse(conf, root))
        {
            memset(&arg, 0, sizeof(arg));
            strcpy(arg.mysql_ip, root["mysql"]["ip"].asCString());
            arg.mysql_port = root["mysql"]["port"].asInt();
            strcpy(arg.mysql_user, root["mysql"]["user"].asCString());
            strcpy(arg.mysql_password, root["mysql"]["password"].asCString());
        }
        conf.close();
    }
  
    void get_message_arg(int &header, UserInfo &body, const char *msg)
    {
        if(reader.parse(msg, root))
        {
            header = root["type"].asInt();
            body.user_name = root["name"].asString();
            body.user_password  = root["pwd"].asString();
        }
    }
private:

    Json::Reader reader;
    Json::Value root;
    ifstream conf;
};










#endif
