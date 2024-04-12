#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "buffer.h"

int main()
{
    int fd = open("./testBuff.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        std::cout << "open funcation error" << std::endl;
        return -1;
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
        return 0;
    }
    return 0;
}