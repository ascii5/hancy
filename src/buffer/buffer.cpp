#include "buffer.h"

int buffer::formatStringAppend(const char* format,va_list arg_list){

    int len = vsnprintf(beginPtr()+writePos,m_buffer.size()-writePos-1,format,arg_list);

    if(len >= m_buffer.size()-writePos-1){
        ensureWriteAble(len);
        return false;
    }

    writePos += len;
    return len;
}

buffer::buffer(int size)
{
    m_buffer.resize(size);
    readPos = 0;
    writePos = 0;
    m_close_log = 0;
}

void buffer::printBuffer()
{
    std::cout << "================================" << std::endl;
    for (int i = readPos; i < writePos; ++i)
    {
        std::cout << m_buffer[i];
    }
    std::cout << std::endl;
    std::cout << "================================" << std::endl;
}

void buffer::pAppend(std::string str)
{
    char buff[200];
    assert(str.size() < 200);
    std::copy(str.c_str(), str.c_str() + str.size(), buff);
    append(buff, str.size());
}

void buffer::hasWritten(size_t len)
{
    writePos += len;
}

char *buffer::peek()
{
    return beginPtr() + readPos;
}

void buffer::ensureWriteAble(size_t len)
{
    if (writeable() < len)
    {
        makeSpace(len);
    }
}

size_t buffer::readable()
{
    return writePos - readPos;
}

void buffer::makeSpace(size_t len)
{
    size_t readAble = readable();

    if (writeable() + prependableBytes() > len)
    {
        std::copy(beginPtr() + readPos, beginPtr() + writePos, beginPtr());
        readPos = 0;
        writePos = readPos + readAble;
    }
    else
    {
        m_buffer.resize(writePos + len + 1);
    }
}

size_t buffer::prependableBytes()
{
    return readPos;
}

void buffer::append(char *buff, int len)
{
    ensureWriteAble(len);
    std::copy(buff, buff + len, beginPtr() + writePos);
    hasWritten(len);
}

size_t buffer::writeable()
{
    return m_buffer.size() - writePos;
}

ssize_t buffer::writeFd(int fd)
{

    size_t readsize = readable();
    ssize_t len = write(fd, peek(), readsize);

    //assert(len >= 0);

    readPos += len;
    return len;
}

ssize_t buffer::readFd(int fd)
{
    char buff[65535];
    struct iovec iov[2];

    size_t writeAble = writeable();

    iov[0].iov_base = beginPtr() + writePos;
    iov[0].iov_len = writeAble;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    int len = readv(fd, iov, 2);
    if (len < 0)
    {
        std::cout << std::strerror(errno) << std::endl;
    }
    else if (static_cast<size_t>(len) < writeAble)
    {
        writePos += len;
    }
    else
    {
        writePos = m_buffer.size();
        append(buff, static_cast<size_t>(len) - writeAble);
    }

    return len;
}

char *buffer::beginPtr()
{
    return &*m_buffer.begin();
}

LINE_STATUS buffer::parseLine()
{

    for (int i = readPos; i < writePos; ++i)
    {
        char temp = m_buffer[i];
        if (temp == '\r')
        {
            if (i + 1 == writePos)
                return LINE_STATUS::LINE_OPEN;
            else if (m_buffer[i + 1] == '\n')
            {
                m_buffer[i] = '\0';
                m_buffer[i + 1] = '\0';
                m_check_index = i + 2;
                return LINE_STATUS::LINE_OK;
            }
            else
                return LINE_STATUS::LINE_BAD;
        }
        else if (temp == '\n')
        {
            if (i - 1 >= 0 && m_buffer[i - 1] == '\r')
            {
                m_buffer[i - 1] = '\0';
                m_buffer[i] = '\0';
                m_check_index = i + 1;
                return LINE_STATUS::LINE_OK;
            }

            return LINE_STATUS::LINE_BAD;
        }
    }

    return LINE_STATUS::LINE_OPEN;
}

char* buffer::getLine(){
    char* temp = peek();
    readPos = m_check_index;
    return temp;
}