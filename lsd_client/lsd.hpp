#ifndef _LSD_HPP_
#define _LSD_HPP_
#include "locker.h"
#include <stdlib.h>
#include <stdio.h>
typedef enum
{
    ERR_CONNECT    = 1,
    ERR_LOGINSEND  = 2,
    ERR_LOGINRECV  = 3,
    ERR_SIGNUPSEND = 4,
    ERR_SIGNUPRECV = 5,
    ERR_CLIENTARG  = 6,
    ERR_UNKNOW     = 7,
    ERR_DISCONNECT = 8
}LSD_ERR;

typedef enum
{
    EN_LOGIN_SUCCESS  = 3,
    EN_LOGIN_ERROR    = 4,
    EN_SIGNUP_SUCCESS = 5,
    EN_SIGNUP_ERROR   = 6,
    EN_PARSE_ERROR    = 7
}MessageHeader;



void set_lsd_errno(LSD_ERR);

class LSD
{
public:
    static LSD * get_lsd(const char *ip = "127.0.0.1", const short port = 8888)
    {   
        if(self == NULL)
        {
            mutex.lock();
            self = new LSD(ip, port);
            mutex.unlock();
        }
        return self;
    }
    
    static void destory_lsd()
    {
        if(self != NULL)
        {
            delete self;
            self = NULL;
        }
    }
    ~LSD();
    
    int login(const char *, const char *);
    int signup(const char *, const char *);
private:
    int init_socket(const char *, const short);
    int do_message(char *, int size);
    static int parse_message(char *);
    int quit();
    LSD(const char *ip = "127.0.0.1", const short port = 8888);
private:
     int sockfd;
     static locker mutex;
     static LSD *self;
};

#endif
