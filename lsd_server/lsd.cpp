#include <stdlib.h>
#include "server.hpp"

int main()
{
    //read configurations for listening
    Server * server = Server::get_server();
    
    //server start
    server->start_server();

    delete server;

    exit(0);
}
