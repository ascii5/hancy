#pragma once

#include<list>
#include<cstdio>
#include<exception>
#include<pthread.h>
#include"../lock/locker.h"
#include"../cgiMysql/mysqlConnPool.h"

template<typename T>
class threadPool{
public:
    threadPool(int actorModel,connectionPool* connPool,int threadNumber = 8,int maxRequest = 10000);
    ~threadPool();
public:
    bool append(T* request,int state);
    bool appendP(T* request);
private:
    static void *worker(void* arg);
    void run();
private:
    int mThreadNumber;
    int mMaxRequests;
    pthread_t* mThreads;
    std::list<T*>mWorkQueue;
    locker mQueueLocker;
    sem mQueueStat;
    connectionPool *mConnPool;
    int mActorModel;
};
template<typename T>
threadPool<T>::threadPool(int actorModel,connectionPool* connPool,int threadNumber = 8,int maxRequest = 10000){
    if(threadNumber <= 0 || maxRequest <= 0){
        throw std::exception();
    }
    mThreads = new pthread_t[threadNumber];
    if(mThreads == NULL)
        throw std::exception();
    for(int i = 0;i < threadNumber;++i){
        if(pthread_create(mThreads+i,NULL,worker,this) != 0){
            delete []mThreads;
            throw std::exception();
        }
        if(pthread_detach(mThreads[i]) != 0){
            delete []mThreads;
            throw std::exception();
        }
    }
}
template<typename T>
threadPool<T>::~threadPool(){
    delete []mThreads;
}
template<typename T>
bool threadPool<T>::append(T* request,int state){
    mQueueLocker.lock();
    if(mWordQueue)
}
template<typename T>
bool threadPool<T>::appendP(T* request){

}
template<typename T>
void* threadPool<T>::worker(void* arg){

}
template<typename T>
void threadPool<T>::run(){

}