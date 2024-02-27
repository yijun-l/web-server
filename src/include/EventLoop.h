#pragma once
#include "Epoll.h"

class EventLoop {
private:
    Epoll* _epoll;
    bool run;
    char buf[1024];

public:
    EventLoop();
    ~EventLoop();
    void loop();
    void addChannel(Channel*);
    void listenHandler(Channel*);
    void upperHandler(Channel*);
};
