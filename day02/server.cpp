// sever.cpp --
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "util.h"
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");
    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr*)&serv_addr,
    sizeof(serv_addr)) == -1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN) == -1,
    "socket listen error");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr,&clnt_addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    std::cout << "new client fd " << clnt_sockfd << "! IP: "
        << inet_ntoa(clnt_addr.sin_addr) << " Port: "
        << ntohs(clnt_addr.sin_port) << std::endl;
    while (true)
    {
        char buf[1024];             // definite buf
        bzero(&buf, sizeof(buf));   // clear buf
        ssize_t read_bytes = read(clnt_sockfd, buf,
            sizeof(buf));
        if (read_bytes > 0)
        {
            std::cout << "message from client fd "
                << clnt_sockfd << " " << buf << std::endl;      // 需要使用endl刷新缓冲区，不然不会显示输出
            write(clnt_sockfd, buf, sizeof(buf));               // 因为没有输入，因此不会触发刷新机制
        }
        else if (read_bytes == 0)
        {
            std::cout << "client fd " << clnt_sockfd
                << " disconnected\n";
            close(clnt_sockfd);
            break;
        }
        else if (read_bytes == -1)
        {
            close(clnt_sockfd);
            errif(true, "socket read error");
        }

    }
    return 0;
}