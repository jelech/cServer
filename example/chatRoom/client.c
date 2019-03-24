#include "client.h"
int main(int argc, char const *argv[])
{

    if(argc < 2) {
        perror("need a name");
    }

    int sockFd, selectId, rsize;
    char rbuf[1024], wbuf[1024], name[100]="[name:]";
    struct sockaddr_in serverAddr, clientAddr;

    if((sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("client socket function error!");
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("192.168.235.1");
    serverAddr.sin_port = htons(6666);

    if(connect(sockFd, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) == -1) {
        perror("connect function error!");
        return 1;
    }

    strcat(name, argv[1]);
    send(sockFd, name, strlen(name), 0);

    fd_set rfd;
    while(1) {
        FD_ZERO(&rfd);
        FD_SET(STDIN_FILENO, &rfd);
        FD_SET(sockFd, &rfd);
        selectId = select(sockFd + 1, &rfd, NULL, NULL, NULL);
        if(selectId == 0 || selectId == -1) {
            perror("select function error!");
            continue;
        }

        if(FD_ISSET(STDIN_FILENO, &rfd)) {
            memset(rbuf, 0, sizeof(rbuf));
            if(rsize = read(STDIN_FILENO, rbuf, sizeof(rbuf)) > 0) {
                send(sockFd, rbuf, sizeof(rbuf), 0);
            }            
        }

        if(FD_ISSET(sockFd, &rfd)) {
            memset(rbuf, 0, sizeof(rbuf));
            if((rsize = recv(sockFd, rbuf, sizeof(rbuf), 0)) > 0) {

                if(!strncmp(rbuf, "quited", 6) && rsize == 6) {
                    close(sockFd);
                    printf("You had quited the chat room\n");
                    return 0;
                }
                printf("%s", rbuf);
                fflush(stdout);
            } 
        }
    }
    return 0;
}
