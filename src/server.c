#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "util.h"

int main(int argc, char* argv[]){
    int lisn_fd, acpt_fd;    /* socket for listening and accepting connection */
    int ret;
    unsigned int sockaddr_len = sizeof(struct sockaddr);
    struct sockaddr_in local_addr, remote_addr;
    char *trans_buffer = (char*)malloc(BUF_SIZE);

    /* build local listen socket address info */
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(local_addr.sin_zero, 8);

    /* socket() - creates a new socket. */
    lisn_fd = socket(AF_INET, SOCK_STREAM, 0);
    serr(lisn_fd, "socket()");

    /* bind() - binds a socket to a specific address and port. */
    ret = bind(lisn_fd, (struct sockaddr*)&local_addr, sockaddr_len);
    serr(ret, "bind()");

    /* listen() - puts a socket in a listening state, waiting for incoming connections. */
    ret = listen(lisn_fd, 10);
    serr(ret, "listen()");

    while(1)
    {
        /* accept() - accepts an incoming connection and creates a new socket for communication. */
        acpt_fd = accept(lisn_fd, (struct sockaddr*)&remote_addr, &sockaddr_len);
        serr(acpt_fd, "accept()");
        printf("client fd %d connected from %s:%d\n", acpt_fd, inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));

        while(1){
            /* recv() - receive data over a socket. */
            ret = recv(acpt_fd, trans_buffer, BUF_SIZE, 0);
            serr(ret, "recv()");
            if (ret == 0){
                break;
            }

            printf("    message from client fd %d: %s\n", acpt_fd, trans_buffer);

            for (int i = 0; i < ret; i++)
            {
                *(trans_buffer + i) = toupper(*(trans_buffer + i));
            }

            /* send() - send data over a socket. */
            send(acpt_fd, trans_buffer, BUF_SIZE, 0);
            bzero(trans_buffer, ret);
        }
        printf("client fd %d dropped\n", acpt_fd);
        close(acpt_fd);
    }
    close(lisn_fd);
    free(trans_buffer);

    return 0;
}
