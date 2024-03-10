#include "config.h"
//端口号,默认9006
int Config::PORT = 9006;
//日志写入方式，默认同步
int Config::LOGWrite = 1;
//触发组合模式,默认listenfd LT + connfd LT
int Config::TRIGMode = 0;
//listenfd触发模式，默认LT
int Config::LISTENTrigmode = 0;
//connfd触发模式，默认LT
int Config::CONNTrigmode = 0;
//优雅关闭链接，默认不使用
int Config::OPT_LINGER = 1;
//数据库连接池数量,默认8
int Config::sql_num = 8;
//线程池内的线程数量,默认8
int Config::thread_num = 8;
//关闭日志,默认不关闭
int Config::close_log = 0;
int Config::logLevel = 2;
//并发模型,默认是proactor
int Config::actor_model = 0;
//需要修改的数据库信息,登录名,密码,库名
std::string Config::m_user = "root";
std::string Config::m_passwd = "123";
std::string Config::m_data_basename = "yourdb";
std::string Config::websiteRoot = "/home/chenranran/Program/hancy/website";
Config::Config(){}
void Config::parse_arg(int argc, char*argv[]){
    int opt;
    const char *str = "p:l:m:o:s:t:c:a:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'p':
        {
            Config::PORT = atoi(optarg);
            break;
        }
        case 'l':
        {
            Config::LOGWrite = atoi(optarg);
            break;
        }
        case 'm':
        {
            Config::TRIGMode = atoi(optarg);
            break;
        }
        case 'o':
        {
            Config::OPT_LINGER = atoi(optarg);
            break;
        }
        case 's':
        {
            Config::sql_num = atoi(optarg);
            break;
        }
        case 't':
        {
            Config::thread_num = atoi(optarg);
            break;
        }
        case 'c':
        {
            Config::close_log = atoi(optarg);
            break;
        }
        case 'a':
        {
            Config::actor_model = atoi(optarg);
            break;
        }
        default:
            break;
        }
    }
}