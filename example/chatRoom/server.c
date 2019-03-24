#include "server.h"

int main(int argc, char const *argv[])
{
    typedef struct client{
        int fd; // 客户端文件描述符
        int isOnline; // 0：下线模式， 1：上线模式
        char name[10]; // 客户端名字(映射IP地址与数据库，可有可无)
    } Client;
    

    int sockFd, sockLen, rsize;
    struct sockaddr_in serverAddr, clientAddr;
    char rbuf[1024], wbuf[1024];
    Client clients[MAX_CLIENT];

    if((sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket function error!");
        return 1;
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clients, 0, sizeof(clients));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(6666);
    sockLen = sizeof(serverAddr);

    if(bind(sockFd, (struct sockaddr*)&serverAddr, sockLen) == -1){
        perror("bind function error");
        return 1;
    }

    if(listen(sockFd,MAX_CLIENT) == -1) {
        perror("listen function error!");
        return 1;
    }

    int selectId, maxFd = sockFd;
    fd_set rfd;
    while(1) {
        FD_ZERO(&rfd);
        FD_SET(sockFd, &rfd);
        for (int i = 0; i < MAX_CLIENT; ++i) {
            if(clients[i].isOnline == 1) {
                FD_SET(clients[i].fd, &rfd);
            }
        }

        selectId = select(maxFd + 1, &rfd, (fd_set *)0, (fd_set *)0, NULL);
        if(selectId == 0 || selectId == -1)
            continue;

        if (FD_ISSET(sockFd, &rfd)) { // socket可以调用，这时候应该accept客户端的请求
            for (int i = 0; i < MAX_CLIENT; ++i) { // 找到一个可以存放的，进行连接
                if(clients[i].fd != 0)  // 一个已经被专用了，不能用
                    continue;
                    
                int len = sizeof(struct sockaddr);
                if ((clients[i].fd = accept(sockFd, (struct sockaddr *)&clientAddr, &len)) == -1) {
                    perror("client accept function error!");
                    break;
                }

                clients[i].isOnline = 1;
                if(clients[i].fd > maxFd)
                    maxFd = clients[i].fd;
                break;
            }
        }

        // 判断是否有新的信息, 处理信息, 判断是否转发给所有人
        for (int i = 0; i < MAX_CLIENT; ++i) {
            if (clients[i].fd == 0)
                continue;
            if (FD_ISSET(clients[i].fd, &rfd)) {
                // printf("debug2:%s_%d:%d\n", "de",i,clients[i].fd);
                memset(rbuf, 0, sizeof(rbuf));
                if((rsize = recv(clients[i].fd, rbuf, sizeof(rbuf), 0)) == 0)
                    continue;

                memset(wbuf, 0, sizeof(wbuf));
                if(!strncmp(rbuf, "[name:]", 7)) { // 获取值为登录
                    sscanf(rbuf, "[name:]%s", clients[i].name);
                    sprintf(wbuf, "%s has enter the room\n", clients[i].name);
                }
                else if(!strncmp(rbuf, "bye", 3) && strlen(rbuf) == 4) {// 获取值为退出
                    send(clients[i].fd, "quited", 6, 0);
                    close(clients[i].fd);
                    clients[i].fd = 0;
                    clients[i].isOnline = 0;
                    sprintf(wbuf, "%s has quit the room\n", clients[i].name);
                }
                else { // 获取值为其他语句
                    sprintf(wbuf, "[%s]:%s", clients[i].name, rbuf);
                }

                printf("receive:%ld:%s", strlen(wbuf), wbuf);
                fflush(stdout);

                for (int j = 0; j < MAX_CLIENT; ++j){ // 转发当前的信息给房间里的所有的人
                    if (clients[j].fd == 0 || clients[j].isOnline == 0 || i == j)
                        continue;
                    send(clients[j].fd, wbuf, strlen(wbuf), 0);
                    
                }
            }

        }
    }
    close(sockFd);
    return 0;
}
