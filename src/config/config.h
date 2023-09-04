#ifndef CONFIG_H
#define CONFIG_H

#include "../threadpool/threadpool.h"
#include "../http/http_conn.h"

using namespace std;

class Config
{
public:
    Config();
    ~Config(){};
public:
    void parse_arg(int argc, char*argv[]);
public:
    //端口号
    int PORT;

    //日志写入方式
    int LOGWrite;

    //触发组合模式
    int TRIGMode;

    //listenfd触发模式
    int LISTENTrigmode;

    //connfd触发模式
    int CONNTrigmode;

    //优雅关闭链接
    int OPT_LINGER;

    //数据库连接池数量
    int sql_num;

    //线程池内的线程数量
    int thread_num;

    //是否关闭日志
    int close_log;

    //并发模型选择
    int actor_model;

    //数据库相关
    string m_data_basename;
    string m_user;
    string m_passwd;

    //网站根目录
    string websiteRoot;
};

#endif