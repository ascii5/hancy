#pragma once

#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<list>
#include<mysql/mysql.h>
#include<error.h>
#include<string.h>
#include<iostream>
#include<string>
#include"../lock/locker.h"
#include"../log/log.h"

class connectionPool{
public:
    MYSQL* getConnection();
    bool releaseConnection(MYSQL* conn);
    int getFreeConn();
    static connectionPool* getInstance();
    void init(std::string url,std::string user,std::string passwd,std::string dataBaseName,int port,int maxConn,int closeLog);
private:
    connectionPool();
    ~connectionPool();
private:
    void destroyPool();
private:
    int mMaxConn;
    int mCurConn;
    int mFreeConn;
    locker lock;
    std::list<MYSQL*>connList;
    sem reserve;
public:
    std::string mUrl;
    std::string mPort;
    std::string mUser;
    std::string mPassword;
    std::string mDataBaseName;
    int mCloseLog;
};

class connectionRAII{
public:
    connectionRAII(MYSQL** con,connectionPool* connPool);
    ~connectionRAII();

private:
    MYSQL* conRAII;
    connectionPool* poolRAII;
};