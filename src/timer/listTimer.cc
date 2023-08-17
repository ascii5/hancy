#include"listTimer.h"

sortTimerList::sortTimerList(){
    head = NULL;
    tail = NULL;
}
sortTimerList::~sortTimerList(){
    utilTimer* cur = head;
    utilTimer* next = head;
    while(cur){
        next = cur -> next;
        delete cur;
        cur = next;
    }
}
void sortTimerList::addTimer(utilTimer* timer){
    if(!timer){
        return ;
    }
    if(!head){
        head = timer;
        tail = timer;
    }
    if(timer -> expire < head -> expire){
        timer -> next = head;
        head -> prev = timer;
        head = timer;
        return;
    }
    addTimer(timer,head);
}
void sortTimerList::addTimer(utilTimer* timer,utilTimer* listHead){
    utilTimer* prev = listHead;
    utilTimer* tmp = prev -> next;
    while(tmp){
        if(timer -> expire < tmp -> expire){
            prev -> next = timer;
            timer -> next = tmp;
            tmp -> prev = timer;
            timer -> prev = prev;
            break;
        }
        prev = tmp;
        tmp = tmp -> next;
    }
    //插入节点在尾节点
    if(!tmp){
        prev -> next = timer;
        timer -> prev = prev;
        timer -> next = NULL;
        tail = timer;
    }
}
void sortTimerList::adjustTimer(utilTimer* timer){
    if(!timer){
        return ;
    }
    utilTimer* tmp = timer -> next;
    if(!tmp || (timer -> expire < tmp -> expire)){
        return ;
    }
    if(timer == head){
        head = head -> next;
        head -> prev = NULL;
        timer -> next = NULL;
        addTimer(timer,head);
    }
    else{
        timer -> prev -> next = timer -> next;
        timer -> next -> prev = timer -> prev;
        addTimer(timer,timer->next);
    }
}
void sortTimerList::delTimer(utilTimer* timer){
    if(!timer){
        return ;
    }
    if((timer == head) && (timer == tail)){
        delete timer;
        head = NULL;
        tail = NULL;
        return ;
    }
    if(timer == head){
        head = head -> next;
        head -> prev = NULL;
        delete timer;
        return ;
    }
    if(timer == tail){
        tail = tail -> prev;
        tail -> next = NULL;
        delete timer;
        return ;
    }
    timer -> prev -> next = timer -> next;
    timer -> next -> prev = timer -> prev;
    delete timer;
}
void sortTimerList::tick(){
    
}
void utils::init(int timeSlot){
    mTIMESLOT = timeSlot;
}
int utils::setNonBlocking(int fd){
    int oldOption = fcntl(fd,F_GETFL);
    int newOption = oldOption | O_NONBLOCK;
    fcntl(fd,F_SETFL,newOption);
    return oldOption;
}
void utils::addfd(int epollfd,int fd,bool oneShot,int TRIGMode){

}
void utils::sigHandler(int sig){

}
void utils::addsig(int sig,void(handler)(int),bool restart){

}
void utils::timerHandler(){

}
void utils::showError(int connfd,const char* info){

}
int *utils::uPipefd = 0;
int utils::uEpollfd = 0;

class utils;
void cbFunc(clientData* userData){

}