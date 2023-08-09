#pragma once
#include<exception>
#include<pthread.h>
#include<semaphore.h>
//采用RAII机制进行封装自动初始化和释放
class sem{
public:
    sem(){
        if(sem_init(&mSem,0,0) != 0){
            throw std::exception();
        }
    };
    sem(int num){
        if(sem_init(&mSem,0,num) != 0){
            throw std::exception();
        }
    };
    ~sem(){
        sem_destroy(&mSem);
    }
public:
    //wait() 信号量减一执行当前线程(信号量不为0)
    bool wait(){
        return sem_wait(&mSem) == 0;
    }
    //post() 信号量加一随即唤醒一个等待中的线程
    bool post(){
        return sem_post(&mSem) == 0;
    }
private:
    sem_t mSem;
};
class locker{
public:
    locker(){

    }
    ~locker(){

    }
public:
    bool lock(){

    }
    bool unLock(){
        
    }
};
class pond{

};