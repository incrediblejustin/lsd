#ifndef _STRUCTURE_HPP_
#define _STRUCTURE_HPP_

#include <string>

typedef struct TcpListenArgument
{
    char listen_ip[32];
    short listen_port;
}ListenArg;

typedef struct MysqlArgument
{
    char mysql_ip[32];
    unsigned int  mysql_port;
    char mysql_user[32];
    char mysql_password[32];
}MysqlArg;

typedef struct MemcachedArgument
{
    char memcached_ip[32];
    unsigned short memcached_port;
}MemcachedArg;

typedef struct UserInformation
{
    std::string user_name;
    std::string user_password;
    const char * get_name() const
    {
        return user_name.c_str();
    }
    const char *get_pwd() const
    {
        return user_password.c_str();
    }
}UserInfo;

typedef enum
{
    EN_LOGIN            = 1,
    EN_SIGNUP           = 2,
    EN_LOGIN_SUCCESS    = 3,
    EN_LOGIN_ERROR      = 4,
    EN_SIGNUP_SUCCESS   = 5,
    EN_SIGNUP_ERROR     = 6,
    EN_PARSE_ERROR      = 7,
    EN_QUIT             = 8
}MessageHeader;

#endif
