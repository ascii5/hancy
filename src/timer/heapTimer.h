#ifndef HEAPTIMER
#define HEAPTIMER


#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <time.h>
#include <functional>
#include <chrono>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "../log/log.h"
#include "../http/http_conn.h"

class util_timer;
struct client_data;

using Clock = std::chrono::high_resolution_clock;
using MS = std::chrono::milliseconds;
using  TimeStamp =  Clock::time_point;
using TimeoutCallback = std::function<void(client_data*)>;



struct client_data
{
    sockaddr_in address;
    int sockfd;
    util_timer *timer;
};

//client_data 可能包含一连串的定时器，所以每个定时器有前后节点，每个定时器有自己的定时事件
class util_timer
{
public:
    int id;  //linux下的文件描述符fd
    time_t expire;
    TimeoutCallback cb_func;
    client_data *user_data;
};

class heapTimer{
public:
    heapTimer(){
        heap_.reserve(64);
    }
    ~heapTimer(){
        heap_.clear();
        ref_.clear();
    }
public:
    void tick();
    void addTimer(util_timer* timer);
    void pop();
    int GetNextTick();
    void doWork(int id);
    void adjust(util_timer* timer);
    int getTimerCount();
    util_timer getFrontTimer();
    void printTimerExpire();
    void delTimer(util_timer* timer);
private:
    void del_(size_t i);   
    void shiftUp_(size_t i);
    bool shiftDown_(size_t index,int n);
    void swapNode_(size_t i,size_t j);
private:
    std::vector<util_timer> heap_;
    std::unordered_map<int,size_t> ref_;
    int flag = 0;
};


class Utils
{
public:
    Utils() {}
    ~Utils() {}
public:
    void init(int timeslot);

    //对文件描述符设置非阻塞
    int setnonblocking(int fd);

    //将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
    void addfd(int epollfd, int fd, bool one_shot, int TRIGMode);

    //信号处理函数
    static void sig_handler(int sig);

    //设置信号函数
    void addsig(int sig, void(handler)(int), bool restart = true);

    //定时处理任务，重新定时以不断触发SIGALRM信号
    void timer_handler();

    void show_error(int connfd, const char *info);

public:
    static int *u_pipefd;
    heapTimer u_heapTimer;
    static int u_epollfd;
    int m_TIMESLOT;
};

class timerManager{
public:
    void add_timer(util_timer* timer);
    void timer_handler();
    void adjust_timer(util_timer *timer);
    void deal_timer(util_timer *timer, int sockfd,client_data* users_timer);
private:
    heapTimer m_heapTimer;
    int m_TIMESLOT;
    int m_close_log = 0;
};

void cb_func(client_data *user_data);

#endif