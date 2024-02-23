#pragma once
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include "util.h"
#include "Channel.h"

#define MAX_EVENTS 128

class Epoll{
private:
    int epfd;
    struct epoll_event *events;

public:
    Epoll();
    ~Epoll();

    void updateChannel(Channel* channel) const;
    void removeChannel(Channel* channel) const;
    std::vector<struct epoll_event> poll (int timeout = -1);
};