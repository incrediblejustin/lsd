#ifndef _MEMCACHED_HPP_
#define _MEMCACHED_HPP_


#include <libmemcached/memcached.h>
#include <time.h>

#include "wrap.hpp"
#include "structure.hpp"

class Memcached
{
public:
    Memcached(const MemcachedArg &arg)
        :st(memcached_create(NULL)),
         server_st(memcached_server_list_append(NULL,
                                                arg.memcached_ip,
                                                arg.memcached_port,
                                                &rt)),
         rt(memcached_server_push(st, server_st))
    {
        if(rt != MEMCACHED_SUCCESS)
        {
            err_moudle("memcached_server_push");
            memcached_free(st);
        }
    }

    ~Memcached()
    {
        memcached_free(st);
    }

    char* query_key(const char *key)
    {
        size_t value_length = 0;
        uint32_t flags = 0;
        char *result = NULL;

        result = memcached_get(st, key, strlen(key), &value_length, &flags, &rt);
        return result;
    }

    void save_value(const char *key, const char *value)
    {
        rt = memcached_set(st, key, strlen(key), value, strlen(value), 
                           (time_t)0, (uint32_t)0);
        if(rt != MEMCACHED_SUCCESS)
             err_moudle("memcached_server_push");
    }
private:
    memcached_st *st;
    memcached_server_st *server_st;
    memcached_return_t rt;
};

#endif 
