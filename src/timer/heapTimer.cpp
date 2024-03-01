#include "heapTimer.h"

void heapTimer::delTimer(util_timer* timer){
    assert(ref_.count(timer -> id) > 0);
    int index = ref_[timer -> id];
    del_(index);
}

void heapTimer::printTimerExpire(){
    assert(!heap_.empty());
    std::time_t timeNow = std::time(nullptr);
    // for(int i = 0;i < heap_.size();++i){
    //     std::cout<<heap_[i].id<<std::endl;
    // }
    // std::cout<<std::endl;
    // std::cout<<std::endl;
    // std::cout<<std::endl;
    // std::cout<<std::endl;
    //int flag = 0;
    while(!heap_.empty()){
        util_timer timer = heap_.front();
        std::cout<<"timer id:   "<<timer.id<<"timer expire:   "<<timer.expire - timeNow<<std::endl;
        // if(flag == 1){
        //     std::cout<<std::endl;
        //     std::cout<<std::endl;
        //     for(int i = 0;i < heap_.size();++i){
        //         std::cout<<"timer id:  "<<heap_[i].id<<std::endl;
        //     }
        //     std::cout<<std::endl;
        //     std::cout<<std::endl;
        // }
        pop();
        flag++;
    }
}

util_timer heapTimer::getFrontTimer(){
    return heap_.front();
}

int heapTimer::getTimerCount(){
    return heap_.size();
}

int heapTimer::GetNextTick(){
    tick();

    int res = -1;
    if(!heap_.empty()){
        util_timer timer = heap_.front();
        auto expire = std::chrono::system_clock::from_time_t(timer.expire);
        res = std::chrono::duration_cast<MS>(expire - Clock::now()).count();
        if(res < 0) res = 0;
    }

    return res;
}

void heapTimer::adjust(util_timer* timer){
    /* 调整指定id的结点 */
    addTimer(timer);
    //assert(!heap_.empty());
    // if(ref_.count(timer->id) == 0){
    //     addTimer(timer);
    // }
    // assert(ref_.count(timer->id) > 0);
    // shiftDown_(ref_[timer->id],heap_.size());
}

void heapTimer::doWork(int id){
    if(heap_.empty() || ref_.count(id) == 0) {
        return;
    }
    size_t i = ref_[id];
    util_timer node = heap_[i];
    node.cb_func(node.user_data);
    del_(i);
}

void heapTimer::del_(size_t i){
    assert(!heap_.empty());
    if(i < 0 || i > heap_.size()){
        std::cout<<"del_ error"<<i<<std::endl;
    }
    assert(i >= 0 && i < heap_.size());
    
    int j = heap_.size() - 1;
    
    // if(flag == 1){
    //     for(int i = 0;i < heap_.size();++i)
    //         std::cout<<"timer id:"<<heap_[i].id<<std::endl;
    // }

    // std::cout<<std::endl;
    // std::cout<<std::endl;
    // std::cout<<std::endl;
    // std::cout<<std::endl;
    if(i < j){
        swapNode_(i,j);
        if(!shiftDown_(i,heap_.size()-1)){
            shiftUp_(i);
        }
    }
    // if(flag == 1){
    //     for(int i = 0;i < heap_.size();++i)
    //         std::cout<<"timer id:"<<heap_[i].id<<std::endl;
    // }
    //std::cout<<"heap_ pop() element id:   "<<heap_.back().id<<std::endl;
    
    ref_.erase(heap_.back().id);
    heap_.pop_back();
}

void heapTimer::pop(){
    assert(!heap_.empty());
    del_(0);
}

void heapTimer::tick(){
    if(heap_.empty())
        return ;
    while(!heap_.empty()){
        util_timer timer = heap_.front();
        auto expire = std::chrono::system_clock::from_time_t(timer.expire);
        if(std::chrono::duration_cast<MS>(expire - Clock::now()).count() > 0) break;
        timer.cb_func(timer.user_data);
        pop();
    }   
}

bool heapTimer::shiftDown_(size_t index,int n){
    assert(index >= 0 && index < n);

    int i = index;
    int j = i * 2 + 1;

    while(j < n){
        if(j + 1 < n && heap_[j+1].expire < heap_[j].expire) j++;
        if(heap_[i].expire < heap_[j].expire)   break;
        swapNode_(i,j);
        i = j;
        j = i * 2 + 1;
    }

    return i > index;
}

void heapTimer::swapNode_(size_t i,size_t j){
    assert(i >= 0 && i < heap_.size());
    assert(j >= 0 && j < heap_.size());
    
    
    ref_[heap_[i].id] = j;
    ref_[heap_[j].id] = i;
    std::swap(heap_[i],heap_[j]);
    
}

void heapTimer::shiftUp_(size_t i){
    assert(i >= 0 && i < heap_.size());
    
    
    int j = (i - 1) / 2;
    while(j >= 0){
        if(heap_[j].expire < heap_[i].expire){ break;}
        swapNode_(i,j);
        i = j;
        j = (i - 1) / 2;
    }
}

void heapTimer::addTimer(util_timer* timer){
    assert(timer->id >= 0);
    
    
    size_t i;
    if(ref_.count(timer->id) == 0){
        i = heap_.size();
        ref_[timer->id] = i;
        heap_.push_back(*timer);
        shiftUp_(i);
    }
    else{
        i = ref_[timer->id];
        
        heap_[i].expire = timer->expire;
        heap_[i].cb_func = timer->cb_func;
        heap_[i].user_data = timer->user_data;

        if(!shiftDown_(i,heap_.size()))
            shiftUp_(i);
    }
}

void Utils::init(int timeslot)
{
    m_TIMESLOT = timeslot;
}

//对文件描述符设置非阻塞
int Utils::setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

//将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
void Utils::addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
{
    epoll_event event;
    event.data.fd = fd;

    if (1 == TRIGMode)
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else
        event.events = EPOLLIN | EPOLLRDHUP;

    if (one_shot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

//信号处理函数
void Utils::sig_handler(int sig)
{
    //为保证函数的可重入性，保留原来的errno
    int save_errno = errno;
    int msg = sig;
    send(u_pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

//设置信号函数
void Utils::addsig(int sig, void(handler)(int), bool restart)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    //在信号处理函数返回后自动继续被中断的系统调用
    if (restart)
        sa.sa_flags |= SA_RESTART;
    //sigfillset() 给指定信号集合添加所有的信号
    //sa.sa_mask 为信号处理期间需要屏蔽的信号，此刻指的是屏蔽所有信号
    sigfillset(&sa.sa_mask);
    //sigaction() 为信号设置新的处理方式
    assert(sigaction(sig, &sa, NULL) != -1);
}

//定时处理任务，重新定时以不断触发SIGALRM信号
void Utils::timer_handler()
{
    u_heapTimer.tick();
    alarm(m_TIMESLOT);
}

void Utils::show_error(int connfd, const char *info)
{
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int *Utils::u_pipefd = 0;
int Utils::u_epollfd = 0;

class Utils;

void cb_func(client_data *user_data)
{

    //std::cout<<"delete user_data"<<std::endl;
    epoll_ctl(Utils::u_epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    http_conn::m_user_count--;
}
