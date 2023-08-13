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
        if(pthread_mutex_init(&mMutex,NULL) != 0){
            throw std::exception();
        }
    }
    ~locker(){
        if(pthread_mutex_destroy(&mMutex) != 0){
            //throw std::exception();
        }
    }
public:
    bool lock(){
        return pthread_mutex_lock(&mMutex) == 0;
    }
    bool unLock(){
        return pthread_mutex_unlock(&mMutex) == 0;
    }
    pthread_mutex_t* getMutex(){
        return &mMutex;
    }
private:
    pthread_mutex_t mMutex;
};
class cond{
public:
    cond(){
        if(pthread_cond_init(&mCond,NULL) != 0){
            throw std::exception();
        }
    }
    ~cond(){
        pthread_cond_destroy(&mCond);
    }
public:
    bool wait(pthread_mutex_t* mMutex){
        return pthread_cond_wait(&mCond,mMutex) == 0;
    }
    bool timewait(pthread_mutex_t* mMutex,struct timespec t){
        int ret = 0;
        ret = pthread_cond_timedwait(&mCond,mMutex,&t);
        return ret == 0;
    }
    bool signal(){
        return pthread_cond_signal(&mCond);
    }
    bool broadcast(){
        return pthread_cond_broadcast(&mCond);
    }
private:
    pthread_cond_t mCond;
};