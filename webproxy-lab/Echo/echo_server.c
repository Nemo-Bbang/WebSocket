#include "../csapp.h"

int main(int argc, char** argv) {
    int listenfd, connfd;
    char* port;
    struct sockaddr_storage client_addr;
    socklen_t client_len;
    char client_hostname[MAXLINE], client_port[MAXLINE];
    char userbuf[MAXLINE], resBuf[MAXLINE];

    rio_t rio;

    if (argc != 2)
    {
        printf("input fuck");
        exit(-1);
    }

    port = argv[1];
    listenfd = Open_listenfd(port);

    if (listenfd < 0)
    {
        printf("socket covert into listen fail.. fuck");
        exit(-1);
    }

    while (1)
    {
        client_len = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA*)&client_addr, &client_len);
        if (connfd < 0) continue;

        Fputs("connected!!\n", stdout);

        rio_readinitb(&rio, connfd);
        while (rio_readlineb(&rio, userbuf, MAXBUF) != 0)
        {
            printf("==================== start of req: %s", userbuf);

            strtok(userbuf, "\n");  // 마지막 줄바꿈 제거
            sprintf(
                resBuf,
                "server: I got your message!\nserver: you said \"%s\"\n%s\n",
                userbuf,
                END_OF_SERVER_RES);  // Format into buf
            rio_writen(connfd, resBuf, strlen(resBuf));
            printf("==================== end of req: %s\n\n", userbuf);
        }
        Close(connfd);
    }
}
