#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "include/util.h"

#define R_PORT 8888
#define R_ADDR "127.0.0.1"

int main() {
    int sockfd;
    int rc;
    struct sockaddr_in remote_addr = {};
    char *trans_buffer = (char*)malloc(BUF_SIZE);

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(R_PORT);
    remote_addr.sin_addr.s_addr = inet_addr(R_ADDR);
    bzero(remote_addr.sin_zero, 8);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serr(sockfd, "socket()");

    rc = connect(sockfd, (struct sockaddr*)&remote_addr, sizeof(remote_addr));
    serr(rc, "connect()");

    printf("connected to %s:%d\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));

    while(true){
        bzero(trans_buffer, BUF_SIZE);

        printf("==> ");
        scanf("%s", trans_buffer);

        rc = send(sockfd, trans_buffer, BUF_SIZE, 0);
        serr(rc, "send()");

        bzero(trans_buffer, BUF_SIZE);

        rc = recv(sockfd, trans_buffer, BUF_SIZE, 0);
        serr(rc, "recv()");
        if (rc == 0){
            break;
        }
        printf("message from server: %s\n", trans_buffer);
    }
    close(sockfd);
    free(trans_buffer);

    return 0;
}