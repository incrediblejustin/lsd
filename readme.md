
# Lsd 登陆注册服务器


### 1. 基本架构

- 使用 **libevent** 作为事件循环检查框架

- 使用 **mysql** 来管理用户数据

- 使用 **memcached** 作为用户数据缓存系统

- 使用 **json** 作为客户端与服务器数据交换的格式

- 使用 **多线程** 作为主要逻辑处理单元

### 2. 服务器的安装与使用

1. 安装

    `$ git clone git@github.com:incrediblejustin/lsd.git`
    
    `$ cd lsd_server`
    
    `$ make`

2. 使用

    `$ ./lsd`
    
3. 注意

    - 由于服务器运行要依赖基本框架中的几个基本框架，所以在安装Lsd之前**必须**安装一下几种库

    - `libevent`, `mysql-server`, `mysql-client`, `libmysqlclient-dev`, `memcached`, `libmemcached-dev`, `jsoncpp`

### 3. 客户端应用程序编程接口的使用

1. 安装

    `$ git clone git@github.com:incrediblejustin/lsd.git`
    `$ cd lsd_client`

2. 使用

    -  **创建lsd对象**
        
    ```cpp
    
    static LSD * get_lsd(const char *ip, const short port );
    
    ```
        
    - **销毁lsd对象**
        
    ```cpp
    
    static void destory_lsd()
    
    ```
    
    - **通过lsd对象完成登陆**
    
    ```cpp
    
    int login(const char *, const char *);
    //返回 0 表示登陆成功
    //返回 -1 表示登陆失败，可以同过全局变量lsd_errno来检查出错类型
    
    ```
    
    - **通过lsd对象完成注册**
    
    ```cpp
    
     int signup(const char *, const char *);
     //返回 0 表示注册成功
     //返回 -1 表示注册失败，可以同过全局变量lsd_errno来检查出错类型
     
    ```
  
3. 注意

    - lsd对象采用的是单例模式，并且是线程安全的

    - 在lsd_client中`test.cpp`有使用举例

      

