#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
using namespace std;
#include <event.h>
#include <string>
#include <unordered_map>
#include <mysql/mysql.h>
#include <event.h>

#include "tcplisten.hpp"
#include "structure.hpp"
#include "json_parse.hpp"
#include "Mysql.hpp"
#include "wrap.hpp"
#include "threadpool.h"
#include "message.hpp"


class Server
{
public:
    ~Server()
    {
        event_base_free(reactor);
        delete listen_server;
    }

    static Server *get_server(const char *ip = NULL, 
            const short port = 0)
    {
        return self == NULL ? (self = new Server(ip, port)) : self;
    }

    Server *halt_server()
    {
        if(self != NULL)
        {
            delete self;
            self = NULL;
        }
    }

    TcpListen *init_tcplisten_from_conf()
    {
        ListenArg arg;
        json_parse.get_listen_arg(arg);
        return new TcpListen(arg);
    }
    void  init_tcplisten(const char *ip = NULL, const short port = 0)
    {
        if(ip == NULL || port == 0)
        {
            if(!(listen_server = init_tcplisten_from_conf()))
            {
                ListenArg listen_arg = {"127.0.0.1", 9877};
                listen_server = new TcpListen(listen_arg);
            }
        }
        if(listen_server == NULL)
            err_moudle("tcplisten moudle");
        else
            cout << "tcplisten moudle start success!" << '\n';
    }

    Mysql * init_mysql_from_conf()
    {
        MysqlArg arg; 
        json_parse.get_mysql_arg(arg);
        return new Mysql(arg);
    }

    void init_mysql()
    {
        if(!(mysql = init_mysql_from_conf()))
        {
            MysqlArg mysql_arg{
                "127.0.0.1",
                3306,
                "sun",
                "111111"
            };
            mysql = new Mysql(mysql_arg);
        }
        if(mysql == NULL)
            err_moudle("mysql moudle");
        else
            cout << "mysql moudle start success!" << '\n';
    }
   
    void init_libevent()
    {
        reactor = event_init(); 
        if(reactor == NULL)
            err_moudle("reactor moudle");
        else
            cout << "reactor  moudle start success!" << '\n';
    }
   
    void init_threadpool()
    {
        try
        {
            pool = new threadpool<Message>;
            cout << "threadpool moudle start success" << '\n';
        }
        catch(...)
        {
            err_moudle("threadpool moudle");
        }
    }
   void start_server()
    {
        struct event * listen_event = event_new(
            reactor,
            listen_server->get_listen_fd(),
            EV_READ | EV_WRITE | EV_PERSIST,
            listen_cb,
            listen_server
            );
        event_add(listen_event, NULL);
        event_base_dispatch(reactor);

        event_free(listen_event);
    }
    
private:
    Server(const char *ip = NULL,const short port = 0) 
    {
        cout << "prepare for threadpool ..." << '\n';
        init_threadpool();
        
        cout<<"prepare for listening ..." << '\n';
        init_tcplisten(ip, port);
        
        cout << "prepare for mysql ..." << '\n';
        init_mysql();


        cout << "prepare for libevent ..." << '\n';
        reactor = event_init();
        cout << "libevent start success ..." << '\n';
        cout << "-------------------------------------------------" << '\n';
        cout << "-------------lsd service start success-----------" << '\n';

    }

    static void client_cb(int fd, short event, void *arg)
    {
        char buffer[1024] = {0};
        int len  = recv(fd, buffer, 1024, 0);
        if(len == -1)
        {
            err_quit("recv");
        }
        if(len == 0)
            return;

        Message message(buffer, self->json_parse, fd, self->mysql);
        self->pool->append(&message);

   }

    static void listen_cb(int fd, short event, void *arg)
    {
        int client_fd = self->listen_server->get_connect_fd();
        struct event *client_event = event_new(
                self->reactor,
                client_fd,
                EV_READ  ,
                client_cb,
                arg
                );
        event_add(client_event, NULL);
    }

private:
    static Server * self;
    Mysql *mysql;    
    TcpListen *listen_server;
    struct event_base *reactor;
    JsonParse json_parse;
    threadpool<Message> *pool;
};
Server *Server::self = NULL;

#endif
