#pragma once

#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdarg.h>
#include<pthread.h>
#include"blockQueue.h"

class log{
public:
    static log* getInstance();
    static void* flushLogThread(void* args);           //异步写
    bool init(const char* fileName ,int closeLog,int logBuffSize = 8192,int splitLines = 5000000,int maxQueueSize = 0);
    void writeLog(int level,const char* format, ...);
    void flush();                                      //刷新缓冲区
private:
    log();
    virtual ~log();
private:
    void* asyncWriteLog();
private:
    char dirName[128];   //路径名
    char logName[128];   //log文件名
    int mSplitLines;     //日志最大行数
    int mLogBuffSize;    //日志缓冲区大小
    long long mCount;    //日志行数记录
    int mToday;          //因为按天分类,当前记录的是那一天
    FILE* mFp;           //打卡log文件的文件指针
    char* mBuf;
    blockQueue<std::string> * mLogQueue;  //阻塞队列
    bool mIsAsync;       //是否同步标志
    locker mMutex;
    int mCloseLog;       //关闭日志
};

#define LOG_DEBUG(format, ...) if(mCloseLog == 0){log::getInstance() -> writeLog(0,format,##__VA_ARGS__);log::getInstance() -> flush();}
#define LOG_INFO(format, ...) if(mCloseLog == 0){log::getInstance() -> writeLog(1,format,##__VA_ARGS__);log::getInstance() -> flush();}
#define LOG_WARN(format, ...) if(mCloseLog == 0){log::getInstance() -> writeLog(2,format,##__VA_ARGS__);log::getInstance() -> flush();}
#define LOG_ERROR(format, ...) if(mCloseLog == 0){log::getInstance() -> writeLog(3,format,##__VA_ARGS__);log::getInstance() -> flush();}