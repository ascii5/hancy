#include "http_conn.h"

// 定义http响应的一些状态信息
const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form = "You do not have permission to get file form this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form = "There was an unusual problem serving the request file.\n";

locker m_lock;
map<string, string> users;

void http_conn::setHeaders()
{
    auto it = m_headers.find("Connection");

    if (it != m_headers.end())
    {
        if (m_headers["Connection"] == "keep-alive")
            m_linger = true;
    }

    it = m_headers.find("Content-Length");

    if (it != m_headers.end())
    {
        m_content_length = std::stoi(m_headers["Content-Length"]);
    }

    it = m_headers.find("Host");
    if (it != m_headers.end())
    {

        m_host = new char[m_headers["Host"].size() + 1];
        std::strcpy(m_host, m_headers["Host"].c_str());
    }

    m_headers.clear();
}

void http_conn::initmysql_result(connection_pool *connPool)
{
    // 先从连接池中取一个连接
    MYSQL *mysql = NULL;
    connectionRAII mysqlcon(&mysql, connPool);

    // 在user表中检索username，passwd数据，浏览器端输入
    if (mysql_query(mysql, "SELECT username,passwd FROM user"))
    {
        LOG_ERROR("SELECT error:%s\n", mysql_error(mysql));
    }

    // 从表中检索完整的结果集
    MYSQL_RES *result = mysql_store_result(mysql);

    // 返回结果集中的列数
    int num_fields = mysql_num_fields(result);

    // 返回所有字段结构的数组
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    // 从结果集中获取下一行，将对应的用户名和密码，存入map中
    while (MYSQL_ROW row = mysql_fetch_row(result))
    {
        string temp1(row[0]);
        string temp2(row[1]);
        users[temp1] = temp2;
    }
}

// 对文件描述符设置非阻塞
int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

// 将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
// EPOLLONESHOT 在监听到事件发生，并有线程处理时，epoll事件删掉注册的epollfd
void addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
{
    epoll_event event;
    event.data.fd = fd;

    if (1 == TRIGMode)
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else
        event.events = EPOLLIN | EPOLLRDHUP;

    if (one_shot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

// 从内核时间表删除描述符
void removefd(int epollfd, int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

// 将事件重置为EPOLLONESHOT
void modfd(int epollfd, int fd, int ev, int TRIGMode)
{
    epoll_event event;
    event.data.fd = fd;

    if (1 == TRIGMode)
        event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    else
        event.events = ev | EPOLLONESHOT | EPOLLRDHUP;

    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

int http_conn::m_user_count = 0;
int http_conn::m_epollfd = -1;

// 关闭连接，关闭一个连接，客户总量减一
void http_conn::close_conn(bool real_close)
{
    if (real_close && (m_sockfd != -1))
    {
        printf("close %d\n", m_sockfd);
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_user_count--;
    }
    // printf("m_user_count:%d\n",m_user_count);
}

// 初始化连接,外部调用初始化套接字地址
void http_conn::init(int sockfd, const sockaddr_in &addr, char *root, int TRIGMode,
                     int close_log, string user, string passwd, string sqlname)
{
    m_sockfd = sockfd;
    m_address = addr;

    addfd(m_epollfd, sockfd, true, m_TRIGMode);
    m_user_count++;

    // 当浏览器出现连接重置时，可能是网站根目录出错或http响应格式出错或者访问的文件中内容完全为空
    m_website_root = root;
    m_TRIGMode = TRIGMode;
    m_close_log = close_log;

    strcpy(sql_user, user.c_str());
    strcpy(sql_passwd, passwd.c_str());
    strcpy(sql_name, sqlname.c_str());

    init();
}

// 初始化新接受的连接
// check_state默认为分析请求行状态
void http_conn::init()
{
    mysql = NULL;
    bytes_to_send = 0;
    bytes_have_send = 0;
    m_check_state = CHECK_STATE_REQUESTLINE;
    m_linger = false;
    m_method = GET;
    m_request_url = 0;
    m_version = 0;
    m_content_length = 0;
    m_host = 0;
    m_start_line = 0;
    m_checked_idx = 0;
    m_read_idx = 0;
    m_write_idx = 0;
    cgi = 0;
    m_state = 0;
    timer_flag = 0;
    is_processed = 0;
    userId = 0;

    memset(m_read_buf, '\0', READ_BUFFER_SIZE);
    memset(m_write_buf, '\0', WRITE_BUFFER_SIZE);
    memset(m_real_file, '\0', FILENAME_LEN);
}

bool http_conn::read_once_buffer(){
    int bytes_read = 0;

    // LT读取数据
    if (0 == m_TRIGMode)
    {
        bytes_read = m_buff->readFd(m_sockfd);
        //bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
        m_read_idx += bytes_read;
        //m_buff->printBuffer();
        if (bytes_read <= 0)
        {
            return false;
        }

        return true;
    }
    // ET读数据
    else
    {
        while (true)
        {
            bytes_read = m_buff->readFd(m_sockfd);
            //bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
            if (bytes_read == -1)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                return false;
            }
            else if (bytes_read == 0)
            {
                return false;
            }
            m_read_idx += bytes_read;
        }
        return true;
    }
}

// 循环读取客户数据，直到无数据可读或对方关闭连接
// 非阻塞ET工作模式下，需要一次性将数据读完
bool http_conn::read_once()
{
    if (m_read_idx >= READ_BUFFER_SIZE)
    {
        return false;
    }
    int bytes_read = 0;

    // LT读取数据
    if (0 == m_TRIGMode)
    {
        bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
        m_read_idx += bytes_read;

        if (bytes_read <= 0)
        {
            return false;
        }

        return true;
    }
    // ET读数据
    else
    {
        while (true)
        {
            bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
            if (bytes_read == -1)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                return false;
            }
            else if (bytes_read == 0)
            {
                return false;
            }
            m_read_idx += bytes_read;
        }
        return true;
    }
}

// 解析http请求行，获得请求方法，目标url及http版本号
http_conn::HTTP_CODE http_conn::parse_request_line(char *text)
{
    std::string requestLine(text);
    regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    smatch submatch;
    if (regex_match(requestLine, submatch, patten))
    {
        std::string stringMethod = submatch[1];
        std::string stringUrl = submatch[2];
        std::string stringVersion = submatch[3];

        if (stringMethod.compare(string("GET")) == 0)
        {
            m_method = GET;
        }
        else if (stringMethod.compare(string("POST")) == 0)
        {
            m_method = POST;
            cgi = 1;
        }
        else
        {
            return BAD_REQUEST;
        }

        // LOG_DEBUG("m_version compelete");
        size_t pos = stringUrl.rfind('/');
        stringUrl = stringUrl.substr(pos);
        // LOG_DEBUG("%s",stringUrl.c_str());
        if (m_request_url != nullptr)
        {
            delete[] m_request_url;
        }
        m_request_url = new char[stringUrl.length() + 1];
        strcpy(m_request_url, stringUrl.c_str());
        m_request_url[stringUrl.length() + 1] = '\0';
        // std::cout<<stringUrl.length()<<"   "<<strlen(m_request_url)
        // LOG_DEBUG("m_request_url complete");

        if (stringVersion.compare("1.1") != 0)
        {
            // std::cout<<<<std::endl;
            return BAD_REQUEST;
        }

        if (strlen(m_request_url) == 1)
        {
            strcat(m_request_url, "index.html");
        }

        m_check_state = CHECK_STATE_HEADER;

        return NO_REQUEST;
    }
    return BAD_REQUEST;
}

// 解析http请求的一个头部信息
http_conn::HTTP_CODE http_conn::parse_headers(char *text)
{
    std::string line(text);
    regex patten("^([^:]*): ?(.*)$");
    smatch subMatch;

    if (regex_match(line, subMatch, patten))
    {
        m_headers[subMatch[1]] = subMatch[2];
        // std::cout<<subMatch[1]<<" "<<subMatch[2]<<std::endl;
    }
    else
    {
        setHeaders();

        if (m_content_length != 0)
        {
            m_check_state = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

// 判断http请求是否被完整读入
http_conn::HTTP_CODE http_conn::parse_content(char *text)
{
    // if (m_read_idx >= (m_content_length + m_checked_idx))
    // {
        text[m_content_length] = '\0';
        // POST请求中最后为输入的用户名和密码
        m_content = text;
        m_buff -> getLine();
        //m_buff -> printBuffer();
        return GET_REQUEST;
    // }
    // return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::process_read()
{
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char *text = 0;

    while ((m_check_state == CHECK_STATE_CONTENT && line_status == LINE_OK) || ((line_status = m_buff->parseLine()) == LINE_OK))
    {
        text = m_buff->getLine();
        
        // std::cout<<std::endl;
        // std::cout<<std::endl;
        // std::cout<<std::endl;

        // std::cout<<text<<std::endl;
        // std::cout<<m_check_state<<std::endl;
        // m_buff -> printBuffer();
        //m_start_line = m_checked_idx;
        // 打印行信息
        LOG_INFO("%s", text);
        switch (m_check_state)
        {
        // 解析请求行
        case CHECK_STATE_REQUESTLINE:
        {
            m_content_length = 0;
            ret = parse_request_line(text);
            // std::cout<<"parse_request_line() compelete"<<std::endl;
            if (ret == BAD_REQUEST)
                return BAD_REQUEST;
            break;
        }
        case CHECK_STATE_HEADER:
        {
            // 解析单个头部信息，解析完成后进行判断是否存在content
            // get请求直接调用do_request进行处理，否则解析content
            // setHeaders();

            ret = parse_headers(text);
            
            if (ret == BAD_REQUEST)
                return BAD_REQUEST;
            else if (ret == GET_REQUEST)
            {
                return do_request();
            }
            break;
        }
        case CHECK_STATE_CONTENT:
        {
            // 使用m_string获取post请求的内容(用户名&密码)
            ret = parse_content(text);
            if (ret == GET_REQUEST)
                return do_request();
            // 解析完成跳出循环
            line_status = LINE_OPEN;
            break;
        }
        default:
            return INTERNAL_ERROR;
        }
    }
    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::do_request()
{
    // m_website_root = root(网站根目录)
    strcpy(m_real_file, m_website_root);
    int len = strlen(m_website_root);
    // printf("m_request_url:%s\n", m_request_url);
    const char *p = strrchr(m_request_url, '/');
    // post请求设置cgi位
    // 处理cgi
    if (cgi == 1 && (*(p + 1) == '2' || *(p + 1) == '3'))
    {

        // 根据标志判断是登录检测还是注册检测
        char flag = m_request_url[1];

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/");
        strcat(m_url_real, m_request_url + 2);
        strncpy(m_real_file + len, m_url_real, FILENAME_LEN - len - 1);
        free(m_url_real);
        // cout<<m_real_file<<endl;
        // 将用户名和密码提取出来
        // user=123&passwd=123
        char name[100], password[100];
        int i;
        for (i = 5; m_content[i] != '&'; ++i)
            name[i - 5] = m_content[i];
        name[i - 5] = '\0';

        int j = 0;
        for (i = i + 10; m_content[i] != '\0'; ++i, ++j)
            password[j] = m_content[i];
        password[j] = '\0';

        if (*(p + 1) == '3')
        {
            // 如果是注册，先检测数据库中是否有重名的
            // 没有重名的，进行增加数据
            char *sql_insert = new char(200);
            strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
            strcat(sql_insert, "'");
            strcat(sql_insert, name);
            strcat(sql_insert, "', '");
            strcat(sql_insert, password);
            strcat(sql_insert, "')");

            // users 是一个map存储密码和用户名
            if (users.find(name) == users.end())
            {
                m_lock.lock();
                int res = mysql_query(mysql, sql_insert);
                users.insert(pair<string, string>(name, password));
                m_lock.unlock();

                if (!res)
                    strcpy(m_request_url, "/html/log.html");
                else
                    strcpy(m_request_url, "/html/registerError.html");
            }
            else
                strcpy(m_request_url, "/html/registerError.html");
            free(sql_insert);
        }
        // 如果是登录，直接判断
        // 若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0
        else if (*(p + 1) == '2')
        {
            if (users.find(name) != users.end() && users[name] == password)
            {
                strcpy(m_request_url, "/html/welcome.html");
                std::string sql_select(R"(SELECT userId FROM user WHERE username = ')");
                sql_select.append(name);
                sql_select.append(R"(';)");
                MYSQL_RES *res;
                MYSQL_ROW row;

                connectionRAII conn(&mysql, connection_pool::GetInstance());
                mysql_query(mysql, sql_select.c_str());
                res = mysql_use_result(mysql);
                row = mysql_fetch_row(res);
                userId = stoi(row[0]);
                mysql_free_result(res);
            }
            else
                strcpy(m_request_url, "/html/logError.html");
        }
        strncpy(m_real_file + len, m_request_url, FILENAME_LEN - len - 1);
        // std::cout<<m_real_file<<std::endl;
    }
    else if (*(p + 1) == '0' && *(p + 2) == '\0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/html/register.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '1' && *(p + 2) == '\0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/html/log.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '5' && *(p + 2) == '\0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/html/picture.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '6' && *(p + 2) == '\0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/html/video.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '7' && *(p + 2) == '\0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/html/fans.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '8' && *(p + 2) == '\0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/html/upload.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    // 文件上传功能实现
    else if (*(p + 1) == '9' && *(p + 2) == '\0')
    {
        if (m_method == POST && m_content_length != 0)
        {

            std::string path(Config::websiteRoot.c_str());
            path.append("/upload");
            std::string content(m_content);
            LOG_INFO("%s", content.c_str());
            std::size_t start = content.find("filename=\"");
            if (start != std::string::npos)
            {
                start += 10;
            }
            std::size_t end = content.find("\"", start);
            std::string fileName = content.substr(start, end - start);

            connectionRAII mySqlConn(&mysql, connection_pool::GetInstance());
            // 判断是否存在同名文件
            MYSQL_RES *res;
            MYSQL_ROW row;
            std::string sql_select(R"(SELECT fileName FROM files;)");
            mysql_query(mysql, sql_select.c_str());
            res = mysql_use_result(mysql);
            bool isSameName = false;
            while ((row = mysql_fetch_row(res)) != NULL)
            {
                // std::cout<<row[0]<<std::endl;
                if (row[0] == fileName)
                {
                    isSameName = true;
                }
            }
            // if(isSameName)
            // std::cout<<"SameFileName"<<std::endl;
            // 数据库增添指定文件信息
            if (!isSameName)
            {
                std::string sql_insert(R"(INSERT INTO files (userId,filename) VALUES ()");

                sql_insert.append(to_string(userId).c_str());
                sql_insert.append(R"(,)");
                sql_insert.append(R"(")");
                sql_insert.append(fileName.c_str());
                sql_insert.append(R"(")");
                sql_insert.append(R"();)");

                mysql_query(mysql, sql_insert.c_str());

                path.append("/");
                path.append(fileName);
                uploadFilePath = path;
                std::string command("touch ");
                command.append(path);
                system(command.c_str());

                std::ofstream outFile(path, std::ofstream::binary | std::ofstream::trunc);
                if (outFile.is_open())
                {
                    std::string fileContent;
                    fileContent = content.substr(end);
                    start = content.find("------WebKitFormBoundary");
                    end = content.find("\r\n", start + strlen("------WebKitFormBoundary"));
                    std::string separotor = content.substr(start, end - start);
                    start = fileContent.find("\r\n\r\n") + strlen("\r\n\r\n");
                    end = fileContent.rfind(separotor.append("--").c_str());
                    fileContent = fileContent.substr(start, end - start - 1);
                    outFile.write(fileContent.c_str(), fileContent.size());
                    outFile.close();
                }
            }
        }
        return FILE_UPLOAD;
    }
    else if (*(p + 1) == 'a' && *(p + 2) == '\0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/html/download.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    else if (*(p + 1) == 'b' && *(p + 2) == '\0')
    {
        // 返回json格式的文件列表
        // 处理下载请求
        return FILE_DOWNLOAD;
    }
    else if (strcasecmp(p, "/index.html") == 0)
    {
        strcat(m_real_file, "/index.html");
    }
    else
    {
        std::string src = "/src";
        strcat(m_real_file, src.c_str());
        len += src.length();
        strncpy(m_real_file + len, p, FILENAME_LEN - len - 1);
    }
    // stat() 用于获取文件的属性存储在stat结构体里面
    if (stat(m_real_file, &m_file_stat) < 0)
    {
        // cout<<"break point"<<endl;
        // cout<<strerror(errno)<<endl;
        // cout<<m_real_file<<endl;
        return NO_RESOURCE;
    }

    if (!(m_file_stat.st_mode & S_IROTH))
        return FORBIDDEN_REQUEST;

    if (S_ISDIR(m_file_stat.st_mode))
        return BAD_REQUEST;

    int fd = open(m_real_file, O_RDONLY);
    // mmap() 将文件映射到内存中提高响应速度
    m_file_address = (char *)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    return FILE_REQUEST;
}
void http_conn::unmap()
{
    if (m_file_address)
    {
        munmap(m_file_address, m_file_stat.st_size);
        m_file_address = 0;
    }
}
bool http_conn::write()
{
    int temp = 0;

    if (bytes_to_send == 0)
    {
        modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
        std::cout<<"++++++++++++++++++"<<std::endl;
        m_buff -> printBuffer();
        std::cout<<"epoll listening"<<std::endl;
        init();
        return true;
    }

    while (1)
    {
        // weitev() 顺序向m_sockfd写入数据(从缓冲区)
        temp = m_buff->writeFd(m_sockfd);

        if (temp < 0)
        {
            // EAGAIN 状态码暂时写完数据
            if (errno == EAGAIN)
            {
                // 重新监听写事件
                modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
                return true;
            }
            unmap();
            return false;
        }

        bytes_have_send += temp;
        bytes_to_send -= temp;
        // // m_iv[0] 的数据已经发送完毕了
        // if (bytes_have_send >= m_iv[0].iov_len)
        // {
        //     m_iv[0].iov_len = 0;
        //     m_iv[1].iov_base = m_file_address + (bytes_have_send - m_write_idx);
        //     m_iv[1].iov_len = bytes_to_send;
        // }
        // else
        // {
        //     m_iv[0].iov_base = m_write_buf + bytes_have_send;
        //     m_iv[0].iov_len = m_iv[0].iov_len - bytes_have_send;
        // }

        if (bytes_to_send <= 0)
        {
            unmap();
            // 重新监听读事件
            modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);

            if (m_linger)
            {
                init();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

// bool http_conn::add_response(const char *format, ...)
// {
//     if (m_write_idx >= WRITE_BUFFER_SIZE)
//         return false;
//     va_list arg_list;
//     va_start(arg_list, format);
//     int len = vsnprintf(m_write_buf + m_write_idx, WRITE_BUFFER_SIZE - 1 - m_write_idx, format, arg_list);
//     if (len >= (WRITE_BUFFER_SIZE - 1 - m_write_idx))
//     {
//         va_end(arg_list);
//         return false;
//     }
//     m_write_idx += len;
//     va_end(arg_list);

//     LOG_INFO("request:%s", m_write_buf);

//     return true;
// }

bool http_conn::add_response(const char*format,...){
    
    va_list args;
    va_start(args,format);
    int len = m_buff->formatStringAppend(format,args);
    va_end(args);
    return true;
}

bool http_conn::add_status_line(int status, const char *title)
{
    return add_response("%s %d %s\r\n", "HTTP/1.1", status, title);
}
bool http_conn::add_headers(int content_len)
{
    return add_content_length(content_len) && add_linger() &&
           add_blank_line();
}
bool http_conn::add_content_length(int content_len)
{
    return add_response("Content-Length:%d\r\n", content_len);
}
bool http_conn::add_content_type()
{
    return add_response("Content-Type:%s\r\n", "text/html");
}
bool http_conn::add_linger()
{
    return add_response("Connection:%s\r\n", (m_linger == true) ? "keep-alive" : "close");
}
bool http_conn::add_blank_line()
{
    return add_response("%s", "\r\n");
}
bool http_conn::add_content(const char *content)
{
    return add_response("%s", content);
}

bool http_conn::process_write(HTTP_CODE ret)
{

    switch (ret)
    {
    case INTERNAL_ERROR:
    {
        add_status_line(500, error_500_title);
        add_headers(strlen(error_500_form));
        if (!add_content(error_500_form))
            return false;
        break;
    }
    case BAD_REQUEST:
    {
        add_status_line(404, error_404_title);
        add_headers(strlen(error_404_form));
        if (!add_content(error_404_form))
        {
            return false;
        }
        break;
    }
    case FORBIDDEN_REQUEST:
    {
        add_status_line(403, error_403_title);
        add_headers(strlen(error_403_form));
        if (!add_content(error_403_form))
            return false;
        break;
    }
    case FILE_REQUEST:
    {
        add_status_line(200, ok_200_title);
        if (m_file_stat.st_size != 0)
        {
            add_headers(m_file_stat.st_size);
            m_buff->append(m_file_address,m_file_stat.st_size);
            // m_iv[0].iov_base = m_write_buf;
            // m_iv[0].iov_len = m_write_idx;
            // m_iv[1].iov_base = m_file_address;
            // m_iv[1].iov_len = m_file_stat.st_size;
            // m_iv_count = 2;
            // bytes_to_send = m_write_idx + m_file_stat.st_size;
            bytes_to_send = m_buff -> readable();
            //m_buff -> printBuffer();
            //std::cout<<"bytes_to_send: "<<bytes_to_send<<std::endl;
            return true;
        }
        else
        {
            const char *ok_string = "<html><body></body></html>";
            add_headers(strlen(ok_string));
            if (!add_content(ok_string))
                return false;
        }
    }
    case FILE_UPLOAD:
    {
        struct stat uploadFileStat;
        stat(uploadFilePath.c_str(), &uploadFileStat);

        add_status_line(200, ok_200_title);
        const char *ok_string = "<html><body>upload sucess</body></html>";
        const char *error_string = "<html><body>upload error</body></html>";
        const char *response_string;
        if (S_ISDIR(uploadFileStat.st_mode) || !(uploadFileStat.st_mode & S_IROTH))
            response_string = error_string;
        else
            response_string = ok_string;
        add_headers(strlen(response_string));
        if (!add_content(response_string))
            return false;
        break;
    }
    case FILE_DOWNLOAD:
    {

        std::ifstream downloadFile(string(m_website_root).append("/upload/test.txt"));

        if (downloadFile.is_open())
        {

            std::string downloadFileContent;
            std::string temp;

            while (std::getline(downloadFile, temp))
            {
                downloadFileContent.append(temp);
            }

            downloadFile.close();

            add_status_line(200, ok_200_title);
            add_headers(downloadFileContent.size());
            if (!add_content(downloadFileContent.c_str()))
            {
                return false;
            }
        }
        else
        {
            add_status_line(500, error_500_title);
            add_headers(strlen(error_500_form));
            if (!add_content(error_500_form))
            {
                return false;
            }
        }
        break;
    }
    case NO_RESOURCE:
    {
        add_status_line(404, error_404_title);
        add_headers(strlen(error_404_form));
        if (!add_content(error_404_form))
            return false;
        break;
    }
    default:
        return false;
    }

    m_iv[0].iov_base = m_write_buf;
    m_iv[0].iov_len = m_write_idx;
    m_iv_count = 1;
    bytes_to_send = m_buff->readable();
    return true;
}

void http_conn::process()
{
    // process_read() 读取(从缓冲区)并判断请求页面(设置m_url)
    

    HTTP_CODE read_ret = process_read();
    test_do_request();
    
    if (read_ret == NO_REQUEST)
    {
        // 监听读事件
        modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
        return;
    }
    
    bool write_ret = process_write(read_ret);
    
    
    //close_conn();
    if (!write_ret)
    {
        close_conn();
    }
    // 监听写事件
    modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
}

void http_conn::test_do_request(){
    int fd = open("./test_do_request.txt",O_RDWR | O_CREAT | O_TRUNC,0644);
    
    if(fd == -1){
        std::cout<<"open function error!"<<std::endl;
    }

    //std::cout<<fd<<std::endl;

    m_buff->writeFd(fd);
    
    close(fd);
}
