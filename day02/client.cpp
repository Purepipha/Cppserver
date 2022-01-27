// client.cpp --
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "util.h"
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    errif(connect(sockfd, (sockaddr*)&serv_addr,
        sizeof(serv_addr)) == -1, "socket connect error");
    while (true)
    {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        std::cin.getline(buf, 1000);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if (write_bytes == -1)
        {
            std::cout << "socket already disconnected, can't write any more!\n";
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
            std::cout << "message from server: " << buf << std::endl;
        else if (read_bytes == 0)
        {
            std::cout << "server socket disconnected!\n";
            break;
        }
        else if (read_bytes == -1)
        {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}