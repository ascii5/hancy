#include"httpconn.h"

#include<mysql/mysql.h>
#include<fstream>

//定义响应的状态消息
const char* ok_200_title = "OK";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntax or is inherently impossible to statisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file from this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form = "There was an unusual problem serving the request file.\n";

locker mLock;
std::map<std::string,std::string>users;
void httpConn::initMysqlResult(connectionPool* connPool){
    MYSQL*  mysql = NULL;
    connectionRAII mysqlcon(&mysql,connPool);

    if(mysql_query(mysql,"SELECT username,passwd FROM user;")){
       LOG_ERROR("SELECT error:%s\n",mysql_error(mysql)); 
    }
    MYSQL_RES * result = mysql_store_result(mysql);
    int numFields = mysql_num_fields(result);
    MYSQL_FIELD* fields = mysql_fetch_fields(result);
    while(MYSQL_ROW row = mysql_fetch_row(result)){
        std::string temp1 = row[0];
        std::string temp2 = row[1];
        users[temp1] = temp2;
    }
}
int setNonBlocking(int fd){
    int oldOption = fcntl(fd,F_GETFL);
    int newOption = oldOption | O_NONBLOCK;
    fcntl(fd,newOption);
    return oldOption;
}
void addfd(int epollfd,int fd,bool oneShot,int TRIGMode){
    epoll_event event;
    event.data.fd = fd;

    if(TRIGMode == 1)
        event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
    else
        event.events = EPOLLIN | EPOLLRDHUP;
    if(oneShot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setNonBlocking(fd);
}
void removefd(int epollfd,int fd){
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}
//将此事件重置为EPOLLONESHOT
void modfd(int epollfd,int fd,int ev,int TRIGMode){
    epoll_event event;
    event.data.fd = fd;

    if(TRIGMode == 1)
        event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    else
        event.events = ev | EPOLLRDHUP | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
int httpConn::mUserCount = 0;
int httpConn::mEpollfd = -1;
void httpConn::closeConn(bool realClose){
    if(realClose && (mSockfd != -1)){
        printf("close %d\n",mSockfd);
        removefd(mEpollfd,mSockfd);
        mSockfd = -1;
        mUserCount--;
    }
}
void httpConn::init(int sockfd,const sockaddr_in &addr,char* root,int TRIGMode,int closeLog,std::string user,std::string passwd,std::string sqlname){
    mSockfd = sockfd;
    mAddress = addr;

    addfd(mEpollfd,sockfd,true,mTRIGMode);
    mUserCount++;
    
    docRoot = root;
    mTRIGMode = TRIGMode;
    mCloseLog = closeLog;
    strcpy(sqlUser,user.c_str());
    strcpy(sqlPasswd,passwd.c_str());
    strcpy(sqlName,sqlname.c_str());
    
    init();
}
void httpConn::init(){
    mysql = NULL;
    bytesToSend = 0;
    bytesHaveSend = 0;
    mCheckState = CHECK_STATE_REQUESTLINE;
    mLinger = false;
    mMethod = GET;
    mUrl = 0;
    mVersion = 0;
    mContentLength = 0;
    mHost = 0;
    mStartLine = 0;
    mCheckedIdx = 0;
    mReadIdx = 0;
    mWriteIdx = 0;
    cgi = 0;
    mState = 0;
    timerFlag = 0;
    improv = 0;

    memset(mReadBUF,'\0',READ_BUFFER_SIZE);
    memset(mWriteBUf,'\0',WRITE_BUFFER_SIZE);
    memset(mRealFile,'\0',FILENAME_LEN);
}

//从状态机
httpConn::LINE_STATUS httpConn::parseLine(){
    char temp;
    for(;mCheckedIdx < mReadIdx;++mCheckedIdx){
        temp = mReadBUF[mCheckedIdx];
        if(temp == '\r'){
            if((mCheckedIdx + 1 == mReadIdx))
                return LINE_OPEN;
            else if(mReadBUF[mCheckedIdx + 1] == '\n'){
                mReadBUF[mCheckedIdx++] = '\0';
                mReadBUF[mCheckedIdx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if(temp == '\n'){
            if(mCheckedIdx > 1 && mReadBUF[mCheckedIdx - 1] == '\r'){
                mReadBUF[mCheckedIdx-1] = '\0';
                mReadBUF[mCheckedIdx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}
bool httpConn::readOnce(){
    if(mReadIdx >= READ_BUFFER_SIZE)
        return false;
    int bytesRead = 0;
    //LT读数据
    if(mTRIGMode == 0){
        bytesRead = recv(mSockfd,mReadBUF + mReadIdx,READ_BUFFER_SIZE - mReadIdx,0);
        mReadIdx += bytesRead;

        if(bytesRead <= 0)
            return false;
        return true;
    }
    //ET读数据
    else{
        while(true){
            bytesRead = recv(mSockfd,mReadBUF + mReadIdx,READ_BUFFER_SIZE - mReadIdx,0);
            if(bytesRead == -1){
                if(errno == EAGAIN | errno == EWOULDBLOCK){
                    break;
                }
                else
                    return false;
            }
            else if(bytesRead == 0)
                return false;
            mReadIdx += bytesRead;
        }
        return true;
    }
}
httpConn::HTTP_CODE httpConn::parseRequestLine(char* text){
    mUrl = strpbrk(text," \t");
    if(!mUrl)
        return BAD_REQUEST;
    *mUrl++ = '\0';
    char* method = text;
    if(strcasecmp(method,"GET") == 0)
        mMethod = GET;
    else if(strcasecmp(method,"POST") == 0)
    {
        mMethod = POST;
        cgi = 1;
    }    
    else
        return BAD_REQUEST;
    mUrl += strspn(mUrl," \t");
    mVersion = strpbrk(mUrl," \t");
    if(!mVersion)
        return BAD_REQUEST;
    *mVersion++ = '\0';
    mVersion += strspn(mVersion," \t");
    if(strcasecmp(mVersion,"HTTP/1.1") != 0)
        return BAD_REQUEST;
    if(strncasecmp(mUrl,"http://",7) == 0){
        mUrl += 7;
        mUrl = strchr(mUrl,'/');
    }
    if(!mUrl || mUrl[0] != '/')
        return BAD_REQUEST;
    if(strlen(mUrl) == 1)
        strcat(mUrl,"judge.html");
    mCheckState = CHECK_STATE_HEADER;
    return NO_REQUEST;
}