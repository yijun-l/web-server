#pragma once
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "util.h"

#define MAX_EVENTS 128

class Epoll{
private:
    int epfd;
    struct epoll_event *events;

public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t event);
    void delFd(int fd);
    std::vector<struct epoll_event> poll (int timeout = -1);
};