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
    if(mWordQueue.size() >= mMaxRequests){
        mQueueLocker.unLock();
        return false;
    }
    request -> mState = state;
    mWorkQueue.push_back(request);
    mQueueLocker.unLock();
    mQueueStat.post();
    return true;
}
template<typename T>
bool threadPool<T>::appendP(T* request){
    mQueueLocker.lock();
    if(mWorkQueue.size() >= mMaxRequests){
        mQueueLocker.unLock();
        return false;
    }
    mWorkQueue.push_back(request);
    mQueueLocker.unLock();
    mQueuestat.post();
    return true;
}
template<typename T>
void* threadPool<T>::worker(void* arg){
    threadPool * pool = (threadPool*)arg;
    pool -> run();
    return pool;
}
template<typename T>
void threadPool<T>::run(){
    while(true){
        mQueueStat.wait();
        mQueueLocker.lock();
        if(mWorkQueue.empty()){
            mQueueLocker.unLock();
            continue;
        }
        T* request = mWorkQueue.front();
        mWorkQueue.pop_front();
        mQueueLocker.unLock();
        if(!request){
            continue;
        }
        if(mActorModel == 1){
            if(request -> mState == 0){
                if(request -> readOnce()){
                    request -> improv = 1;
                    connectionRAII mysqlcon(&request->mysql,mConnPool);
                    request -> process();
                }
                else{
                    request -> improv = 1;
                    request -> timerFlag = 1;
                }
            }
        }
        else{
            if(request -> write()){
                request -> improv = 1;
            }
            else{
                request -> improv = 1;
                request -> timerFlag = 1;
            }
        }
    }
    else{
        connectionRAII mysqlConn(&request -> mysql,mConnPool);
        request -> process();
    }
}