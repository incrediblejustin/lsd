#ifndef _TCP_LISTEN_HPP_
#define _TCP_LISTEN_HPP_
#include <iostream>
using namespace std;
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "wrap.hpp"
#include "structure.hpp"
const int BACKLOG = 16;


class TcpListen
{
public:
    TcpListen(const ListenArg &arg)
    {
        listen_fd = do_listen(arg.listen_ip, arg.listen_port);
    }
   
    ~TcpListen()
    {
        close(listen_fd);
    }

    int do_listen(const char *ip,const short  port)
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd == -1)
        {
            err_quit("socket");
        }
       
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port);
        int res = -1;
        if(res == inet_pton(AF_INET,ip,&addr.sin_addr))
        {
            err_quit("inet_pton ip");
        }
        if(res == bind(fd, (struct sockaddr*)&addr, sizeof(addr)))
        {
            err_quit("bind");
        }
        if(res == listen(fd,BACKLOG ))
        {
            err_quit("listen");
        }
        return fd;
    }

    int get_listen_fd()
    {
        return listen_fd;
    }

    int get_connect_fd()
    {
        struct sockaddr_in client;
        socklen_t socklen = sizeof(client);

        int conn = accept(listen_fd, (struct sockaddr*)&client, &socklen);
        if(conn == -1)
        {
            err_quit("accept");
        }
        else
            return conn;
    }
private:
    int listen_fd;
};

#endif
