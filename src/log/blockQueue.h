#pragma once

#include<iostream>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>
#include"../lock/locker.h"
template <typename T>
class blockQueue{
public:
    blockQueue(int maxSize);
    ~blockQueue();
public:
    bool full();
    bool empty();
    void clear();
    bool front(T& value);
    bool back(T& value);
    int size();
    int maxSize();
    bool push(const T& iteam);
    bool pop(T& iteam);
    bool pop(T& iteam,int msTimeout);
private:
    locker mMutex;
    cond mCond;

    T* mArry;
    int mSize;     //当前的数组大小
    int mMaxSize;  //最大数组大小
    int mFront;
    int mBack;
};





template <typename T>
blockQueue<T>::blockQueue(int maxSize){
    if(maxSize <= 0){
        std::cout<<"maxSize is too small"<<std::endl;
        exit(-1);
    }
    mMaxSize = maxSize;
    mArry = new T[mMaxSize];
    mSize = 0;
    mFront = -1;
    mBack = -1;
}
template <typename T>
blockQueue<T>::~blockQueue(){
    mMutex.lock();
    if(mArry != NULL)
        delete mArry;
    mMutex.unLock();
}
template <typename T>
bool blockQueue<T>::full(){
    mMutex.lock();
    if(mSize >= mMaxSize){
        mMutex.unLock();
        return true;
    }
    mMutex.unLock();
    return false;
}
template <typename T>
void blockQueue<T>::clear(){
    mMutex.lock();
    mSize = 0;
    mFront = -1;
    mBack = -1;
    mMutex.unLock();
};
template <typename T>
bool blockQueue<T>::empty(){
    mMutex.lock();
    if(mSize == 0){
        mMutex.unLock();
        return true;
    }
    mMutex.unLock();
    return false;
}
template <typename T>
bool blockQueue<T>::front(T& value){
    mMutex.lock();
    if(mSize == 0){
        mMutex.unLock();
        return false;
    }
    value = mArry[mFront];
    mMutex.unLock();
    return true;
}
template <typename T>
bool blockQueue<T>::back(T& value){
    mMutex.lock();
    if(mSize == 0){
        mMutex.unLock();
        return false;
    }
    value = mArry[mBack];
    mMutex.unLock();
    return true;
}
template <typename T>
int blockQueue<T>::size(){
    int temp = 0;
    mMutex.lock();
    temp = mSize;
    mMutex.unLock();
    return temp;
}
template <typename T>
int blockQueue<T>::maxSize(){
    int temp = 0;
    mMutex.lock();
    temp = mMaxSize;
    mMutex.unLock();
    return temp;
}
template <typename T>
bool blockQueue<T>::push(const T& iteam){
    mMutex.lock();
    if(mSize >= mMaxSize){
        mCond.broadcast();
        mMutex.unLock();
        return false;
    }
    mBack = (mBack + 1) % mMaxSize;
    mArry[mBack] = iteam;
    mSize++;
    mCond.broadcast();
    mMutex.unLock();
    return true;
}
template <typename T>
bool blockQueue<T>::pop(T& iteam){
    mMutex.lock();
    while(mSize <= 0){
        if(!mCond.wait(mMutex.getMutex())){
            mMutex.unLock();
            return false;
        }
    }
    mFront = (mFront + 1) % mMaxSize;
    iteam = mArry[mFront];
    mSize--;

    mMutex.unLock();
    return true;
}
//添加超时处理的重载版本
template <typename T>
bool blockQueue<T>::pop(T& iteam,int msTimeout){
    struct timespec t{0,0};
    struct timeval now{0,0};
    gettimeofday(&now,NULL);
    mMutex.lock();
    if(mSize <= 0){
        t.tv_sec = now.tv_sec + (msTimeout / 1000);
        t.tv_nsec = (msTimeout % 1000) * 1000;
        if(!mCond.timewait(mMutex.getMutex(),t)){
            mMutex.unLock();
            return false;
        }
    }
    if(mSize <= 0){
        mMutex.unLock();
        return false;
    }
    mFront = (mFront + 1) % mMaxSize;
    iteam = mArry[mFront];
    mSize--;
    mMutex.unLock();
    return true;
}