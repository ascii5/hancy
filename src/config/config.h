#ifndef CONFIG_H
#define CONFIG_H

#include <cstdlib>
#include <string>
#include <unistd.h>
using namespace std;

class Config
{
public:
    Config();
    ~Config(){};
public:
    static void parse_arg(int argc, char*argv[]);
public:
    //端口号
    static int PORT;

    //日志写入方式
    static int LOGWrite;

    //触发组合模式
    static int TRIGMode;

    //listenfd触发模式
    static int LISTENTrigmode;

    //connfd触发模式
    static int CONNTrigmode;

    //优雅关闭链接
    static int OPT_LINGER;

    //数据库连接池数量
    static int sql_num;

    //线程池内的线程数量
    static int thread_num;

    //是否关闭日志
    static int close_log;

    //并发模型选择
    static int actor_model;

    //数据库相关
    static string m_data_basename;
    static string m_user;
    static string m_passwd;

    //网站根目录
    static string websiteRoot;
};

#endif