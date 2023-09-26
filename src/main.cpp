#include "./webserver/webserver.h"
#include "./config/config.h"

int main(int argc, char *argv[])
{
    //命令行解析
    Config::parse_arg(argc,argv);
    WebServer server;
    
    server.WebServerPrepare();
    server.eventLoop();

    return 0;
}