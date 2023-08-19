#pragma once

#include<unistd.h>
#include<signal.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<stdarg.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/uio.h>
#include<map>

#include"../lock/locker.h"
#include"../cgiMysql/mysqlConnPool.h"
#include"../timer/listTimer.h"
#include"../log/log.h"

class httpConn{
public:
    static const int FILENAME_LEN = 200;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024; 
    enum METHOD{
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };
    enum CHECK_STATE{
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    enum HTTP_CODE{
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSETD_CONNECTION
    };
    enum LINE_STATUS{
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };
public:
    httpConn(){};
    ~httpConn(){};
public:
    void init(int sockfd,const sockaddr_in &addr,char*,int,int, std::string user,std::string passwd,std::string sqlName);
    void closeConn(bool realClose = true);
    void process();
    bool readOnce();
    bool write();
    sockaddr_in* getAddress(){
        return &mAddress;
    }
    void initMysqlResult(connectionPool* connPool);
    int timerFlag;
    int improv;
private:
    void init();
    HTTP_CODE processRead();
    bool processWrite(HTTP_CODE ret);
    HTTP_CODE parseRequestLine(char* text); 
    HTTP_CODE parseHeaders(char* text);
    HTTP_CODE parseContent(char* text);
    HTTP_CODE doRequest();
    char* getLine(){return mReadBUF + mStartLine;};
    LINE_STATUS parseLine();
    void unmap();
    bool addResponse(const char* format,...);
    bool addContent(const char* content);
    bool addStatusLine(int status,const char* title);
    bool addHeaders(int contentLength);
    bool addContentType();
    bool addContentLength(int contentLength);
    bool addLinger();
    bool addBlankLine();
public:
    static int mEpollfd;
    static int mUserCount;
    MYSQL* mysql;
    int mState;
private:
    int mSockfd;
    sockaddr_in mAddress;
    char mReadBUF[READ_BUFFER_SIZE];
    long mReadIdx;
    long mCheckedIdx;
    int mStartLine;
    char mWriteBUf[WRITE_BUFFER_SIZE];
    int mWriteIdx;
    CHECK_STATE mCheckState;
    METHOD mMethod;
    char mRealFile[FILENAME_LEN];
    char* mUrl;
    char* mVersion;
    char* mHost;
    long mContentLength;
    bool mLinger;
    char* mFileAddress;
    
    struct stat mFileStat;
    struct iovec mIv[2];
    int mIvCount;
    int cgi;
    char* mString;
    int bytesToSend;
    int bytesHaveSend;
    char* docRoot;
    
    std::map<std::string,std::string> mUsers;
    int mTRIGMode;
    int mCloseLog;

    char sqlUser[100];
    char sqlPasswd[100];
    char sqlname[100];
};