#include "webserver.h"

WebServer::WebServer()
{
    //http_conn类对象
    users = new http_conn[MAX_FD];


    m_root = (char *)malloc(strlen(Config::websiteRoot.c_str())+1);
    strcpy(m_root,Config::websiteRoot.c_str());
    
    
    //定时器
    users_timer = new client_data[MAX_FD];

    m_port = Config::PORT;
    m_user = Config::m_user;
    m_passWord = Config::m_passwd;
    m_databaseName = Config::m_data_basename;
    m_sql_num = Config::sql_num;
    m_thread_num = Config::thread_num;
    m_log_write = Config::LOGWrite;
    m_OPT_LINGER = Config::OPT_LINGER;
    m_TRIGMode = Config::TRIGMode;
    m_close_log = Config::close_log;
    m_actormodel = Config::actor_model;
    logLevel = Config::logLevel;
}

WebServer::~WebServer()
{
    close(m_epollfd);
    close(m_listenfd);
    close(m_pipefd[1]);
    close(m_pipefd[0]);
    delete[] users;
    delete[] users_timer;
    delete m_pool;
}

void WebServer::WebServerPrepare(){
    this -> log_write();
    this -> sql_pool();
    this -> thread_pool();
    this -> trig_mode();
    this -> eventListen();
}

void WebServer::trig_mode()
{
    //LT + LT
    if (0 == m_TRIGMode)
    {
        m_LISTENTrigmode = 0;
        m_CONNTrigmode = 0;
    }
    //LT + ET
    else if (1 == m_TRIGMode)
    {
        m_LISTENTrigmode = 0;
        m_CONNTrigmode = 1;
    }
    //ET + LT
    else if (2 == m_TRIGMode)
    {
        m_LISTENTrigmode = 1;
        m_CONNTrigmode = 0;
    }
    //ET + ET
    else if (3 == m_TRIGMode)
    {
        m_LISTENTrigmode = 1;
        m_CONNTrigmode = 1;
    }
}

void WebServer::log_write()
{
    if (0 == m_close_log)
    {
        //初始化日志
        //m_log_write == 1 为异步模式
        if (1 == m_log_write)
            Log::get_instance()->init("./ServerLog", m_close_log, 2000, 800000, 800,logLevel);
        else
            Log::get_instance()->init("./ServerLog", m_close_log, 2000, 800000, 0,logLevel);
    }
}

void WebServer::sql_pool()
{
    //初始化数据库连接池
    m_connPool = connection_pool::GetInstance();
    //connectionPool 采用一个链表来存储连接(连接池)
    //循环创建mysql连接加入到链表中
    m_connPool->init("localhost", m_user, m_passWord, m_databaseName, 3306, m_sql_num,m_close_log);

    //初始化数据库读取表(httpconn中的map)
    users->initmysql_result(m_connPool);
}

void WebServer::thread_pool()
{
    //线程池
    //m_actormodel 并发模型 proactor 和 reactor
    //m_connPool 数据库连接池
    //m_threadNum 线程数量
    //初始化线程池
    m_pool = new threadpool<http_conn>(m_actormodel, m_connPool, m_thread_num);
}

void WebServer::eventListen()
{
    //网络编程基础步骤(祖传)
    m_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(m_listenfd >= 0);

    struct linger tmp;
    if(m_OPT_LINGER == 0){
        tmp.l_onoff = 0;
        tmp.l_linger = 1;
    }
    else{
        tmp.l_onoff = 1;
        tmp.l_linger = 1;
    }
    setsockopt(m_listenfd,SOL_SOCKET,SO_LINGER,&tmp,sizeof(tmp));


    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    //INADDR_ANY 接受任意ip地址的连接
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(m_port);

    int flag = 1;
    //允许端口重复绑定
    setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    ret = bind(m_listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret >= 0);
    ret = listen(m_listenfd, 5);
    assert(ret >= 0);

    utils.init(TIMESLOT);

    //epoll创建内核事件表
    epoll_event events[MAX_EVENT_NUMBER];
    m_epollfd = epoll_create(5);
    assert(m_epollfd != -1);

    utils.addfd(m_epollfd, m_listenfd, false, m_LISTENTrigmode);
    http_conn::m_epollfd = m_epollfd;

    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, m_pipefd);
    assert(ret != -1);
    utils.setnonblocking(m_pipefd[1]);
    //监听m_pipefd[0]
    utils.addfd(m_epollfd, m_pipefd[0], false, 0);


    //为信号添加信号处理函数
    //SIG_ALRM 定时器超时
    //SIG_TERM 程序中断
    //SIGPIPE 信号是当进程向已经关闭的管道或者套接字写入数据的时候触发1
    utils.addsig(SIGPIPE, SIG_IGN);
    //向u_pipefd[0] 发送信号信息
    utils.addsig(SIGALRM, utils.sig_handler, false);
    utils.addsig(SIGTERM, utils.sig_handler, false);

    //alarm函数会在指定的时间间隔(TIMESLOT)发送一个SIG_ALARM信号
    alarm(TIMESLOT);

    //工具类,信号和描述符基础操作
    Utils::u_pipefd = m_pipefd;
    Utils::u_epollfd = m_epollfd;
}

void WebServer::timer(int connfd, struct sockaddr_in client_address)
{

    //初始化相对应的http_conn对象
    users[connfd].init(connfd, client_address, m_root, m_CONNTrigmode, m_close_log, m_user, m_passWord, m_databaseName);

    //初始化client_data数据
    //创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
    users_timer[connfd].address = client_address;
    users_timer[connfd].sockfd = connfd;
    util_timer* timer = new util_timer;
    timer->user_data = &users_timer[connfd];
    //cb_func() epoll内核删除sockfd, 关闭对应的sockfd
    timer->cb_func = cb_func;
    //time(NULL) 表示获取当前时间
    //设置定时器的时间(绝对时间)
    time_t cur = time(NULL);
    timer->expire = cur + 3 * TIMESLOT;
    timer->id = connfd;
    users_timer[connfd].timer = timer;
    m_timer_manager.add_timer(timer);

    //delete timer;
}

//若有数据传输，则将定时器往后延迟3个单位
//并对新的定时器在链表上的位置进行调整
bool WebServer::dealclinetdata()
{
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    //0为LT 1为ET
    if (0 == m_LISTENTrigmode)
    {
        int connfd = accept(m_listenfd, (struct sockaddr *)&client_address, &client_addrlength);
        if (connfd < 0)
        {
            LOG_ERROR("%s:errno is:%d", "accept error", errno);
            return false;
        }
        if (http_conn::m_user_count >= MAX_FD)
        {
            utils.show_error(connfd, "Internal server busy");
            LOG_ERROR("%s", "Internal server busy");
            return false;
        }
        //timer() 添加定时器
        timer(connfd, client_address);
    }

    else
    {
        while (1)
        {
            int connfd = accept(m_listenfd, (struct sockaddr *)&client_address, &client_addrlength);
            if (connfd < 0)
            {
                LOG_ERROR("%s:errno is:%d", "accept error", errno);
                break;
            }
            if (http_conn::m_user_count >= MAX_FD)
            {
                utils.show_error(connfd, "Internal server busy");
                LOG_ERROR("%s", "Internal server busy");
                break;
            }
            timer(connfd, client_address);
        }
        return false;
    }
    return true;
}

bool WebServer::dealwithsignal(bool &timeout, bool &stop_server)
{
    int ret = 0;
    int sig;
    char signals[1024];
    ret = recv(m_pipefd[0], signals, sizeof(signals), 0);
    if (ret == -1)
    {
        return false;
    }
    else if (ret == 0)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < ret; ++i)
        {
            switch (signals[i])
            {
            case SIGALRM:
            {
                timeout = true;
                break;
            }
            case SIGTERM:
            {
                stop_server = true;
                break;
            }
            }
        }
    }
    return true;
}

void WebServer::dealwithread(int sockfd)
{
    util_timer *timer = users_timer[sockfd].timer;

    //reactor 工作线程io数据
    if (1 == m_actormodel)
    {
        if (timer)
        {
            //将定时器往后延迟三个单位(TIMESLOT)
            m_timer_manager.adjust_timer(timer);
        }

        //若监测到读事件，将该事件放入请求队列
        m_pool->append(users + sockfd, 0);
        //users httpconn类型数组
        //httpconn::is_processed 的意思是是否被处理过
        while (true)
        {
            if (1 == users[sockfd].is_processed)
            {
                if (1 == users[sockfd].timer_flag)
                {
                    //关闭连接，删除timer
                    m_timer_manager.deal_timer(timer, sockfd,users_timer);
                    users[sockfd].timer_flag = 0;
                }
                users[sockfd].is_processed = 0;
                break;
            }
        }
    }
    else
    {
        //模拟proactor 主线程负责io数据工作线程只负责工作逻辑
        //真 proactor 是操作系统io数据，工作线程只负责工作逻辑
       
        
        if (users[sockfd].read_once_buffer())
        {
            LOG_INFO("deal with the client(%s)", inet_ntoa(users[sockfd].get_address()->sin_addr));

            //若监测到读事件，将该事件放入请求队列
            m_pool->append_p(users + sockfd);

            if (timer)
            {
                m_timer_manager.adjust_timer(timer);
            }
        }
        else
        {
            m_timer_manager.deal_timer(timer, sockfd,users_timer);
            //dealtime() 删除定时器，sockfd(关闭)
        }
    }
}

void WebServer::dealwithwrite(int sockfd)
{
    util_timer *timer = users_timer[sockfd].timer;
    //reactor
    if (1 == m_actormodel)
    {
        if (timer)
        {
            m_timer_manager.adjust_timer(timer);
        }

        m_pool->append(users + sockfd, 1);

        while (true)
        {
            if (1 == users[sockfd].is_processed)
            {
                if (1 == users[sockfd].timer_flag)
                {
                    m_timer_manager.deal_timer(timer, sockfd,users_timer);
                    users[sockfd].timer_flag = 0;
                }
                users[sockfd].is_processed = 0;
                break;
            }
        }
    }
    else
    {
        //proactor
        if (users[sockfd].write())
        {
            LOG_INFO("send data to the client(%s)", inet_ntoa(users[sockfd].get_address()->sin_addr));

            if (timer)
            {
                m_timer_manager.adjust_timer(timer);
            }
        }
        else
        {
            m_timer_manager.deal_timer(timer, sockfd,users_timer);
        }
    }
}

void WebServer::eventLoop()
{
    bool timeout = false;
    bool stop_server = false;
  
 
    //统一事件源
    while (!stop_server)
    {
        int number = epoll_wait(m_epollfd, events, MAX_EVENT_NUMBER, -1);
        if (number < 0 && errno != EINTR)
        {
            LOG_ERROR("%s", "epoll failure");
            break;
        }

        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;

            //处理新到的客户连接
            if (sockfd == m_listenfd)
            {
                //dealclinetdata() 取出新到的客户连接，并且设置定时器
                bool flag = dealclinetdata();
                if (false == flag)
                    continue;
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                //EPOLLRDHUP 对端关闭连接
                //EPOLLHUP 发生错误 连接异常中断
                //EPOLLERR 发生错误事件
                
                //服务器端关闭连接，移除对应的定时器
                //epoll中删除对应的sockfd,关闭对应的sockfd
                util_timer *timer = users_timer[sockfd].timer;
                m_timer_manager.deal_timer(timer, sockfd,users_timer);
            }
            //处理信号(m_pipefd[0] 存在数据可读)
            //SIG_ALARM 为 timeout = true
            //SIG_TERM 为 stop_server = true;
            else if ((sockfd == m_pipefd[0]) && (events[i].events & EPOLLIN))
            {
                bool flag = dealwithsignal(timeout, stop_server);
                if (false == flag)
                    LOG_ERROR("%s", "dealclientdata failure");
            }
            //处理客户连接上接收到的数据
            else if (events[i].events & EPOLLIN)
            {
                //dealwithread() 
                dealwithread(sockfd);
            }
            else if (events[i].events & EPOLLOUT)
            {
                dealwithwrite(sockfd);
            }
        }
        if (timeout)
        {
            //调用tick清理函数，清理链表上的过期连接
            m_timer_manager.timer_handler();

            LOG_INFO("%s", "timer tick");

            //清理后重新给 timeout 赋值为 false 
            timeout = false;
        }
    }
}