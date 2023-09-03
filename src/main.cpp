#include "./webserver/webserver.h"
#include "./config/config.h"

int main(int argc, char *argv[])
{
    //命令行解析
    Config config;
    config.parse_arg(argc, argv);

    WebServer server(config);
    
    
    server.WebServerPrepare();
    server.eventLoop();

    return 0;
}