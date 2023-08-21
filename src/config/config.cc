#include"config.h"
config::config(){
    port = 9006;
    logWrite = 0;
    trigMode = 0;
    listenTrigMode = 0;
    connTrigMode = 0;
    optLinger = 0;
    sqlNum = 8;
    threadNum = 8;
    closeLog = 0;
    actorMode = 0;
}
void config::parse_arg(int argc,char* argv[]){
    int opt;
    const char* str = "p:l:m:o:s:t:c:a:";
    while((opt = getopt(argc,argv,str)) != -1){
        switch (opt)
        {
        case 'p':
            port = atoi(optarg);
            break;
        case 'l':
            logWrite = atoi(optarg);
            break;
        case 'm':
            trigMode = atoi(optarg);
            break;
        case 'o':
            optLinger = atoi(optarg);
            break;
        case 's':
            sqlNum = atoi(optarg);
            break;
        case 't':
            threadNum = atoi(optarg);
            break;
        case 'c':
            closeLog = atoi(optarg);
            break;
        case 'a':
            actorMode = atoi(optarg);
            break;
        default:
            std::cout<<"Unknow paramemeter!"<<std::endl;
            exit(-1);
        }
    }
}