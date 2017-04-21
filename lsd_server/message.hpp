#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include "structure.hpp"
#include "json_parse.hpp"
#include "Mysql.hpp"
class Message
{
public:
    typedef int UserType;
    Message(const char *msg, JsonParse &json_parse, 
            const int fd, Mysql *msq)
        :client_fd(fd),
         mysql(msq)
    {
        json_parse.get_message_arg(header, body, msg);
    }

    void process()
    {
        bool ret = false;
        switch(header)
        {
        case EN_LOGIN:
            {
                ret = do_login();
                if(ret)
                    do_anwser(EN_LOGIN_SUCCESS);
                else
                    do_anwser(EN_LOGIN_ERROR); 
            }
            break;   
        case EN_SIGNUP:
            {    
                ret = do_signup();
                if(ret)
                    do_anwser(EN_SIGNUP_SUCCESS);
                else
                    do_anwser(EN_SIGNUP_ERROR);
            }
            break;
        case EN_QUIT:
            {
                close(client_fd);
            }
            break;
        default:
            {    
                 do_anwser(EN_PARSE_ERROR);
            }
            break;
        }
    }
    bool do_login()
    {
        return mysql->query_login(body);
    }
    
    bool do_signup()
    {
        return mysql->query_signup(body);
    }

    void do_anwser(MessageHeader anwser)
    {
        Json::Value root;
        root["type"] = anwser;
        string status = root.toStyledString();
        int res = send(client_fd, status.c_str(), status.length(), 0);
        if(res == -1)
            err_quit("send");
    }
private:
    int       client_fd;
    Mysql    *mysql;
    UserType  header;
    UserInfo  body;
};

#endif
