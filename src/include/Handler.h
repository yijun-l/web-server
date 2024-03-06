#pragma once
#include "Channel.h"

class EventLoop;
class Handler {
public:
    Handler();
    ~Handler();
    void HandlerEntry(Channel* channel);
    void setEventLoop(EventLoop* _event_loop);

private:
    void listenHandler(Channel* channel);
    void upperHandler(Channel* channel);
    EventLoop* event_loop{};
};

