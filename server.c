#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 4096

/**
 * http://www.cnblogs.com/skynet/archive/2010/12/12/1903949.html
 */
int main(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[4096];
    int n;
    int pid;

    /* 获取文件描述符 */
    if ((listenfd = socket(AF_INET /* IPV4 */, SOCK_STREAM /* Socket 类型 */, 0 /* 0 => 根据 Socket 类型自动选择协议，这里是 TCP */)) == -1)
    {
        /* 获取文件描述副失败，退出程序 */
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    /* init servaddr， 将这个结构体的内容都赋为 0 */
    memset(&servaddr, 0, sizeof(servaddr));
    // IPV4
    servaddr.sin_family = AF_INET;
    /* address in network byte order */
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 6666 端口
    servaddr.sin_port = htons(6666);

    // 绑定文件描述符到 sockaddr_in 结构体
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    // listen函数的第一个参数即为要监听的socket描述字，第二个参数为相应socket可以排队的最大连接个数
    if (listen(listenfd, 10) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    printf("======waiting for client's request======\n");
    while (1)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
            continue;
        }
        // 下面的操作交给子进程去处理
        pid = fork();
        // 主进程继续处理下一个请求
        if (!pid)
        {
            continue;
        }
        n = recv(connfd, buff, MAXLINE, 0);
        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);
        close(connfd);
    }

    close(listenfd);
    return 0;
}