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

}
void sortTimerList::delTimer(utilTimer* timer){

}
void sortTimerList::tick(){

}
void utils::init(int timeSlot){
    mTIMESLOT = timeSlot;
}
int utils::setNonBlocking(int fd){
    int oldOption = fcntl(fd,F_GETF)
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