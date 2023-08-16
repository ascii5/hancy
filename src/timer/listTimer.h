#pragma once

#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<stdarg.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/uio.h>
#include<time.h>
#include"../log/log.h"

class utilTimer;
struct clientData
{
    sockaddr_in address;
    int sockfd;
    utilTimer* timer;
};

class utilTimer{
public:
    utilTimer():prev(NULL),next(NULL){}
public:
    time_t expire;
    void(* cbFunc)(clientData*);
    clientData* userData;
    utilTimer* prev;
    utilTimer* next;
};

class sortTimerList{
public:
    sortTimerList();
    ~sortTimerList();
public:
    void addTimer(utilTimer* timer);
    void adjustTimer(utilTimer* timer);
    void delTimer(utilTimer* timer);
    void tick();
private:
    void addTimer(utilTimer* timer,utilTimer* listHeader);
private:
    utilTimer* head;
    utilTimer* tail;
};

class utils{
public:
    utils(){};
    ~utils(){};
public:
    void init(int timeSlot);
    int setNonBlocking(int fd);
    void addfd(int epollfd,int fd,bool oneShot,int TRIGMode);
    static void sigHandler(int sig);
    void addsig(int sig,void(handler)(int),bool restart = true);
    void timerHandler();
    void showError(int connfd,const char* info);
public:
    static int * uPipefd;
    sortTimerList mTimerList;
    static int uEpollfd;
    int mTIMESLOT;
};

void cbFunc(clientData* userData);

