#ifndef _MYSQL_HPP_
#define _MYSQL_HPP_

#include <mysql/mysql.h>
#include <string.h>

#include "wrap.hpp"
#include "structure.hpp"
#include "Memcached.hpp"
const char *DB_NAME = "sun";

class Mysql
{

public:
    Mysql(const MysqlArg &arg) 
    {
        conn = mysql_init((MYSQL*)0);
        if(conn == NULL)
        {
            err_moudle("mysql_init");
        }

        if(!mysql_real_connect(
                    conn, 
                    arg.mysql_ip,
                    arg.mysql_user,
                    arg.mysql_password,
                    DB_NAME,
                    arg.mysql_port,
                    NULL,// unix_socket_name
                    0    // flags
                    ))
        {
            err_moudle("mysql_real_connect");
        }
        cout << "prepare for memcached ..." << endl;
        init_memcached_client();
    }

    Memcached * init_memcached_client_from_conf()
    {
        JsonParse json_parse;
        MemcachedArg arg;
        json_parse.get_memcached_arg(arg);
        return new Memcached(arg);
    }

    void init_memcached_client()
    {
        if(!(memca = init_memcached_client_from_conf()))
        {
            MemcachedArg memcached_arg = {"127.0.0.1", 11211};
            memca = new Memcached(memcached_arg);
        }
        if(memca == NULL)
        {
            err_moudle("init_memcached_client");
        }
        else
            cout << "memcached moudle start success!" << endl;
    }
   
    ~Mysql()
    {
        if(res != NULL)
            mysql_free_result(res);
        if(conn != NULL)
            mysql_close(conn);
    }
    
    bool query_from_memcached_login(const UserInfo& user)  
    {
        bool query_flag = false;
        char *value = memca->query_key(user.get_name());
        if(value != NULL && strcmp(value, user.get_pwd()) == 0)
        {
            query_flag = true;
        }
        return query_flag;
    }
  
    bool query_from_mysql_login(const UserInfo &user)
    {
        bool query_flag = false;
        char query_sql[128] = {0};
        sprintf(query_sql, 
                "SELECT * FROM Client WHERE name='%s' and pwd='%s';",
                user.get_name(),
                user.get_pwd());

        if(!mysql_real_query(conn, query_sql, strlen(query_sql)))
        {
            res = mysql_store_result(conn);
            while(row = mysql_fetch_row(res))
            {
                if(strcmp(user.get_name(), row[0]) == 0 &&
                   strcmp(user.get_pwd(), row[1]) == 0)
                {
                    memca->save_value(user.get_name(), user.get_pwd());
                    query_flag = true;
                }                      
            }
        }
        return query_flag;
    }
   
    bool query_login(const UserInfo &user)
    {
        bool query_flag = false;

        query_flag = query_from_memcached_login(user);
        if(!query_flag)
            query_flag = query_from_mysql_login(user);

        return query_flag;
    }

    bool query_ifused_from_memcached(const UserInfo &user)
    {
        if(memca->query_key(user.get_name()))
            return true;
        else
            return false;
    }

    bool query_ifused_from_mysql(const UserInfo &user)
    {
        char query_sql[128] = {0};
        sprintf(query_sql, 
                "SELECT name FROM Client WHERE name='%s';",
                user.get_name());
        if(mysql_real_query(conn, query_sql, strlen(query_sql)))
            err_moudle("query_ifused_from_mysql");
  
        res = mysql_store_result(conn);


        if(row = mysql_fetch_row(res))
            return true;
        else
            return false;
    }
    bool query_signup(const UserInfo &user)
    {
        bool query_flag = false;

        //search from memcached if user already exist 
        cout << 1 << endl;
        if(query_ifused_from_memcached(user))
            return query_flag;
                
        //search from mysql if user already exist
        cout << 2 << endl;
        if(query_ifused_from_mysql(user))
            return query_flag;
        
        //user signup into mysql & save value to memcached
        char query_sql[128] = {0};
        sprintf(query_sql, 
                "INSERT INTO Client values('%s', '%s');",
                user.get_name(),
                user.get_pwd());
        if(!mysql_real_query(conn,query_sql,strlen(query_sql)))
        {
            memca->save_value(user.get_name(), user.get_pwd());
            query_flag = true;
        }
        else
        {
            err_moudle("query_signup");
        }
        return query_flag;

    }    
    
private:
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    Memcached *memca;
};

#endif
