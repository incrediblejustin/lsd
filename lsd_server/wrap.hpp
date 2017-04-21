#ifndef _WRAP_HPP_
#define _WRAP_HPP_
#include <iostream>
using namespace std;

#include <errno.h>
#include <string.h>
#include <stdlib.h>


void err_quit(const char *err)
{
    cerr << err << " error: " << errno << strerror(errno) << endl;
    exit(-1);
}

void err_moudle(const char *moudle)
{
    cerr << moudle << "start error!";
    exit(-1);
}


#endif
