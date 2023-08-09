#pragma once
#include<unistd.h>
#include<string.h>
#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<stdlib.h>
class config{
public:
    config();
    ~config(){};
public:
    void parse_arg(int argc,char* argv[]);
public:
    int port;
    int logWrite;
    int trigMode;
    int listenTrigMode;
    int connTrigMode;
    int optLinger;
    int sqlNum;
    int threadNum;
    int closeLog;
    int actorMode;
};