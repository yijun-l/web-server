#pragma once
#include "Epoll.h"
#include "Handler.h"

class EventLoop {
private:
    Epoll* epoll;
    Handler* handler;
    bool run;

public:
    explicit EventLoop(Handler* _handler);
    ~EventLoop();
    void loop();
    void addChannel(Channel*);
    void delChannel(Channel*);
};
