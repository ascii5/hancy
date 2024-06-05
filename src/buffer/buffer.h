#include <vector>
#include <string>
#include <cassert>
#include <atomic>
#include <sys/uio.h>
#include <cstring>
#include <iostream>
#include <sys/unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdarg>
#include "../log/log.h"

enum LINE_STATUS
{
    LINE_OK = 0,
    LINE_BAD,
    LINE_OPEN
};

class buffer
{
public:
    buffer(int size = 1024);
    ~buffer() = default;

public:
    

public:
    ssize_t readFd(int fd);
    ssize_t writeFd(int fd);

public:
    void pAppend(std::string str);
    int formatStringAppend(const char* format,va_list va_list);
    void printBuffer();
    LINE_STATUS parseLine();
    char* getLine();
    void append(char *buff, int len);
    size_t readable();
private:
    void hasWritten(size_t len);
    void ensureWriteAble(size_t len);
    char *beginPtr();
    char *peek();
    size_t writeable();
    size_t prependableBytes();
    void makeSpace(size_t len);
    
private:
    std::vector<char> m_buffer;
    std::atomic<std::size_t> readPos;
    std::atomic<std::size_t> writePos;
    LINE_STATUS status;
    size_t m_check_index;
    int m_close_log;
};