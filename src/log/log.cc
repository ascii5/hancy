#include"log.h"

log::log(){
    mCount = 0;
    mIsAsync = false;
}
log::~log(){
    if(mFp != NULL){
        fclose(mFp);
    }
}


log* log::getInstance(){
    static log instance;
    return &instance;
}
void* log::flushLogThread(void* args){
    log::getInstance() -> asyncWriteLog();
}
bool log::init(const char* fileName ,int closeLog,int logBuffSize,int splitLines,int maxQueueSize){
    if(maxQueueSize >= 1){
        mIsAsync = true;
        mLogQueue = new blockQueue<std::string>(maxQueueSize);
        pthread_t tid;
        pthread_create(&tid,NULL,flushLogThread,NULL); 
    }
    
    mCloseLog = closeLog;
    mLogBuffSize = logBuffSize;
    mBuf = new char[mLogBuffSize];
    memset(mBuf,'/0',sizeof(mBuf));
    mSplitLines = splitLines;

    time_t t = time(NULL);
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = * sys_tm;
    const char* p = strrchr(fileName,'/');
    char logFullName[256] = {0};
    if(p == NULL){
        snprintf(logFullName,255,"%d_%02d_%02d_%s",my_tm.tm_year+1900,my_tm.tm_mon+1,my_tm.tm_mday,fileName);
    }
    else{
        strcpy(logName,p + 1);
        strncpy(dirName,fileName,p - fileName + 1);
        snprintf(logFullName,255,"%s%d_%02d_%02d_%s",my_tm.tm_year+1900,my_tm.tm_mon+1,my_tm.tm_mday,logName);
    }
    mToday = my_tm.tm_mday;
    mFp = fopen(logFullName,"a");
    if(mFp == NULL){
        return false;
    }
    return true;
}
void log::writeLog(int level,const char* format, ...){
    struct timeval now = {0,0};
    gettimeofday(&now,NULL);
    time_t t = now.tv_sec;
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    char s[16] = {0};
    switch (level)
    {
    case 0:
        strcpy(s,"[debug]:");
        break;
    case 1:
        strcpy(s,"[info]:");
        break;
    case 2:
        strcpy(s,"[warn]:");
        break;
    case 3:
        strcpy(s,"[error]:");
        break;
    default:
        strcpy(s,"[info]:");
        break;
    }
    mMutex.lock();
    mCount++;

    if(mToday != my_tm.tm_mday || mCount % mSplitLines == 0){
        char newLog[256] = {0};
        fflush(mFp);
        fclose(mFp);
        char tail[16] = {0};

        snprintf(tail,16,"%d_%02d_%02d",my_tm.tm_year + 1900,my_tm.tm_mon + 1,my_tm.tm_mday);
        if(mToday != my_tm.tm_mday){
            snprintf(newLog,255,"%s%s%s",dirName,tail,logName);
            mToday = my_tm.tm_mday;
            mCount = 0;
        }
        else{
            snprintf(newLog,255,"%s%s%s.%lld",dirName,tail,logName,mCount/mSplitLines);
            mFp = fopen(newLog,"a");
        }
    }
    mMutex.unLock();

    va_list valst;
    va_start(valst,format);

    std::string logStr;
    mMutex.lock();

    int n = snprintf(mBuf, 48, "%d-%02d-%02d %02d:%02d:%02d.%06ld %s ",
                     my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday,
                     my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, now.tv_usec, s);

    int m = vsnprintf(mBuf+n,mLogBuffSize - n -1,format,valst );
    mBuf[n+m] = '\n';
    mBuf[n+m+1] = '\0';
    logStr = mBuf;
    
    mMutex.unLock();
    if(mIsAsync && !mLogQueue->full()){
        mLogQueue -> push(logStr);
    }
    else{
        mMutex.lock();
        fputs(logStr.c_str(),mFp);
        mMutex.unLock();
    }

    va_end(valst);
}   
void log::flush(){
    mMutex.lock();
    fflush(mFp);
    mMutex.unLock();
}
void* log::asyncWriteLog(){
    std::string single_log;
    while(mLogQueue -> pop(single_log)){
        mMutex.lock();
        fputs(single_log.c_str(),mFp);
        mMutex.unLock();
    }
}