#include<vector>
#include<string>
#include<cassert>
#include<atomic>
#include<sys/uio.h>
#include<cstring>
#include<iostream>
#include<sys/unistd.h>
#include<sys/types.h>
#include<unistd.h>


class buffer{
public:
    buffer(int size = 1024);
    ~buffer() = default;
public:
    ssize_t readFd(int fd);
    ssize_t writeFd(int fd);
public:
    void pAppend(std::string str);
    void printBuffer();
    void clear();
    void printAll();
private:
    void hasWritten(size_t len);
    void ensureWriteAble(size_t len);
    char* beginPtr();
    char* peek();
    size_t writeable();
    void append(char* buff,int len); 
    size_t prependableBytes();
    void makeSpace(size_t len);
    size_t readable();
private:
    std::vector<char> m_buffer;
    std::atomic<std::size_t> readPos;
    std::atomic<std::size_t> writePos;
};