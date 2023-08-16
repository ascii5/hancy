#include"mysqlConnPool.h"

connectionPool::connectionPool(){
    mFreeConn = 0;
    mCurConn = 0;
}
connectionPool* connectionPool::getInstance(){
    static connectionPool myConnPool;
    return &myConnPool;
}
void connectionPool::init(std::string url,std::string user,std::string passwd,std::string DBName,int port,int maxConn,int closeLog){
    mUrl = url;
    mPort = port;
    mUser = user;
    mPassword = passwd;
    mDataBaseName = DBName;
    mCloseLog = closeLog;

    for(int i = 0;i < maxConn;++i){
        MYSQL* conn = NULL;
        conn = mysql_init(conn);

        if(conn == NULL){
            LOG_ERROR("MYSQL ERROR");
            exit(-1);
        }
        conn = mysql_real_connect(conn,url.c_str(),user.c_str(),passwd.c_str(),DBName.c_str(),port,NULL,0);

        if(conn == NULL){
            LOG_ERROR("MYSQL ERROR");
            exit(-1);
        }
        connList.push_back(conn);
        ++mFreeConn;
    }
    reserve = sem(mFreeConn);
    mMaxConn = mFreeConn;
}
MYSQL* connectionPool::getConnection(){
    MYSQL* conn = NULL;
    if(connList.size() == 0){
        return conn;
    }
    reserve.wait();
    lock.lock();

    conn = connList.front();
    connList.pop_front();

    --mFreeConn;
    ++mCurConn;

    lock.unLock();
    return conn;
}
bool connectionPool::releaseConnection(MYSQL* conn){
    if(conn == NULL){
        return false;
    }
    lock.lock();
    connList.push_back(conn);
    ++mFreeConn;
    --mCurConn;
    lock.unLock();
    reserve.post();
    return true;
}
void connectionPool::destroyPool(){
    lock.lock();
    if(connList.size() > 0){
        std::list<MYSQL*>::iterator it;
        for(it = connList.begin();it != connList.end();++it){
            MYSQL* conn = *it;
            mysql_close(conn);
        }
        mCurConn = 0;
        mFreeConn = 0;
        connList.clear();

    }
    lock.unLock();
}
int connectionPool::getFreeConn(){
    return this -> mFreeConn;
}
connectionPool::~connectionPool(){
    destroyPool();
}
connectionRAII::connectionRAII(MYSQL** SQL,connectionPool* connPool){
    *SQL = connPool -> getConnection();

    conRAII = *SQL;
    poolRAII = connPool;
}
connectionRAII::~connectionRAII(){
    poolRAII -> releaseConnection(conRAII);
}