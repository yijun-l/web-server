#include "include/Epoll.h"

Epoll::Epoll() : epfd(-1), events(nullptr){
    epfd = epoll_create(1);
    serr(epfd, "epoll_create()");
    events = new struct epoll_event[MAX_EVENTS];
    memset(events, 0, sizeof(events));
}

Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

void Epoll::addFd(int fd, uint32_t event) {
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;

    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    serr(ret, "epoll_ctl() add listen fd");
}

void Epoll::delFd(int fd){
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    serr(ret, "epoll_ctl() delete client fd");
}

std::vector<epoll_event> Epoll::poll(int timeout){
    std::vector<epoll_event> activeEvents;
    int fds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    serr(fds, "epoll_wait()");
    for(int i = 0; i < fds; i++){
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
}

