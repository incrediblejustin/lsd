#include "lsd.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>


LSD_ERR lsd_errno;
locker glb_mutex;
void set_lsd_errno(LSD_ERR lsd_err)
{
    glb_mutex.lock();
    lsd_errno = lsd_err;
    glb_mutex.unlock();
}

LSD *LSD::self = NULL;
locker LSD::mutex = locker();
LSD::LSD(const char *ip, const short port)
    : sockfd(-1)
{
    int ret = init_socket(ip, port);
    if(ret == -1)
    {
        set_lsd_errno(ERR_CONNECT);
    }
}

int LSD::init_socket(const char *ip = "127.0.0.1", const short port = 8888)
{
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if(sockfd == -1)
   {
       set_lsd_errno(ERR_CONNECT);
       return -1;
   }

   struct sockaddr_in addr;
   bzero(&addr, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_port   = htons(port);
   
   int ret = inet_pton(AF_INET, ip, &addr.sin_addr);
   if(ret == -1)
   {
       close(sockfd);
       set_lsd_errno(ERR_CONNECT);
       return -1;
   }

   ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
   if(ret == -1)
   {
       close(sockfd);
       set_lsd_errno(ERR_CONNECT);
       return -1;
   }
   return 0;
}



int LSD::do_message(char *message, int size )
{
    int nsend = send(sockfd, message, strlen(message), 0);
    if(nsend <= 0)
    {
        set_lsd_errno(ERR_LOGINSEND);
        return -1;
    }
    bzero(message, size);
    int nrecv = recv(sockfd, message, size, 0);
    if(nrecv < 0)
    {
        set_lsd_errno(ERR_LOGINRECV);
        return -1;
    }
    return 0;
}

int LSD::parse_message(char *message)
{
    size_t len = strlen("{\n   \"type\" : ");
    if(strlen(message) <= len)
        return EN_PARSE_ERROR;
    return *(message + len) - 0x30;
}
int LSD::login(const char *name, const char *pwd)
{
    if(strlen(name) <= 0 || strlen(pwd) <= 0)
    {
        set_lsd_errno(ERR_CLIENTARG);
        return -1;
    }
    char message[512] = {0}; 
    sprintf(message,"{\n\"type\":1,\n\"name\":\"%s\",\n\"pwd\":\"%s\"\n}",
            name, pwd);
    int ret = do_message(message, 512);
    if(ret == -1)
        return ret;

    int type = parse_message(message);
    if(type == EN_LOGIN_SUCCESS)
    {
        return 0;
    }
    else if(type == EN_PARSE_ERROR)
    {
        set_lsd_errno(ERR_CLIENTARG);
        return -1;
    }

    set_lsd_errno(ERR_UNKNOW);
    return -1;

}

int LSD::signup(const char *name, const char *pwd)
{
    if(strlen(name) <= 0 || strlen(pwd) <= 0)
    {
        set_lsd_errno(ERR_CLIENTARG);
        return -1;
    }
    char message[512] = {0};
    sprintf(message,"{\n\"type\":2,\n\"name\":\"%s\",\n\"pwd\":\"%s\"\n}",
            name, pwd);
    int ret = do_message(message, 512);
    if(ret == -1)
        return ret;

    int type = parse_message(message);
    if(type == EN_SIGNUP_SUCCESS)
    {
        return 0;
    }
    else if(type == EN_PARSE_ERROR)
    {
        set_lsd_errno(ERR_CLIENTARG);
        return -1;
    }

    set_lsd_errno(ERR_UNKNOW);
    return -1;

}

int LSD::quit()
{
    char message[512] = {0};
    sprintf(message,"{\n\"type\":8,\n\"name\":\"%s\",\n\"pwd\":\"%s\"\n}",
            "empty", "empty");
    int ret = do_message(message, 512); 
    return ret;
}


LSD::~LSD()
{
    int ret = quit();
    if(ret == -1)
        set_lsd_errno(ERR_DISCONNECT);
    close(sockfd);
}

