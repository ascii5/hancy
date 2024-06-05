#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "buffer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void testBufferReadAndWrite()
{
    int fd = open("./testBuff.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        std::cout << "open funcation error" << std::endl;
        return;
    }

    buffer buff(1);

    for (int i = 0; i < 100; ++i)
        buff.pAppend(std::string("hello world\n"));

    buff.writeFd(fd);

    lseek(fd, 0, SEEK_SET);

    buff.readFd(fd);

    buff.printBuffer();

    if (close(fd) == -1)
    {
        std::cout << std::strerror(errno) << std::endl;
        return;
    }
}

void testBufferParseLine()
{
    buffer buff(100);

    std::string str("123");
    str.append("\r");
    str.append("\n");
    str.append("123");

    buff.pAppend(str);

    if (buff.parseLine() == LINE_OK)
    {
        std::cout << "parseLine sucessful" << std::endl;
    }
}

void testBufferGetLine()
{
    buffer buff(100);

    std::string str("123");
    str.append("\r");
    str.append("\n");
    str.append("123");

    buff.pAppend(str);

    if (buff.parseLine() == LINE_OK)
    {
        std::cout << buff.getLine() << std::endl;
    }
}


void testBufferWithNetWork(int port)
{
    int mListenFd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    int ret;
    ret = bind(mListenFd, (struct sockaddr *)&address, sizeof(address));
    assert(ret >= 0);
    ret = listen(mListenFd, 5);
    assert(ret >= 0);

    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientFd = accept(mListenFd, (sockaddr *)&clientAddress, &clientAddressSize);
    assert(clientFd != -1);

    buffer buff(1024);
    buff.readFd(clientFd);

    while(buff.parseLine() == LINE_OK){
        std::cout<<buff.getLine()<<std::endl;
    }

    close(clientFd);
    close(mListenFd);
}

void testFormatString(const char* format,...){
    buffer m_buff(100);
    
    int fd = open("./testBuff.txt",O_RDWR | O_CREAT | O_TRUNC,0644);
    if(fd == -1){
        std::cout<<"open function error!"<<std::endl;
    }

    va_list args;
    va_start(args,format);

    m_buff.formatStringAppend(format,args);
    
    va_end(args);

    m_buff.writeFd(fd);
    close(fd);
}

void  testReadable(){
    buffer m_buff(100);
    m_buff.pAppend(string("this is a str"));

    size_t size = m_buff.readable();
    std::cout<<"size :"<<size<<std::endl;

}   

int main()
{
    // testFormatString("this is %d test",1);
    testReadable();

    return 0;
}