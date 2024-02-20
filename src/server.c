#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "util.h"

int main(int argc, char* argv[]){
    int lisn_fd, acpt_fd;    /* socket for listening and accepting connection */
    int ret, fds;
    unsigned int sockaddr_len = sizeof(struct sockaddr);
    struct sockaddr_in local_addr, remote_addr;
    char *trans_buffer = (char*)malloc(BUF_SIZE);

    int epfd;    /* fd for epoll instance */
    struct epoll_event events[MAX_EVENTS], ev;

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

    setNonBlocking(lisn_fd);    /* add non-blocking to listen fd */
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = lisn_fd;

    epfd = epoll_create(1);
    serr(epfd, "epoll_create()");

    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lisn_fd, &ev);
    serr(ret, "epoll_ctl() add listen fd");

    while(1)
    {
        fds = epoll_wait(epfd, events, MAX_EVENTS, EPOLL_TIMER);
        serr(fds, "epoll_wait()");

        for(int i = 0; i < fds; i++){
            if(events[i].data.fd == lisn_fd){
                /* accept() - accepts an incoming connection and creates a new socket for communication. */
                acpt_fd = accept(lisn_fd, (struct sockaddr*)&remote_addr, &sockaddr_len);
                serr(acpt_fd, "accept()");
                printf("client fd %d connected from %s:%d\n", acpt_fd, inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));

                setNonBlocking(acpt_fd);    /* add non-blocking to listen fd */
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = acpt_fd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, acpt_fd, &ev);
                serr(ret, "epoll_ctl() add client fd");
            } else if (events[i].events & EPOLLIN){
                acpt_fd = events[i].data.fd;
                /* recv() - receive data over a socket. */
                ret = recv(acpt_fd, trans_buffer, BUF_SIZE, 0);
                serr(ret, "recv()");
                if (ret == 0){
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, acpt_fd, NULL);
                    serr(ret, "epoll_ctl() delete client fd");
                    printf("client fd %d dropped\n", acpt_fd);
                    close(acpt_fd);
                    continue;
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
        }
    }
    close(lisn_fd);
    free(trans_buffer);

    return 0;
}
