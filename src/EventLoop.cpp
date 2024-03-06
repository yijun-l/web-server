#include "include/EventLoop.h"

EventLoop::EventLoop(Handler* _handler)
    : epoll(new Epoll()),
    handler(_handler),
    run(true){
}

EventLoop::~EventLoop() {
    delete epoll;
}

void EventLoop::addChannel(Channel* channel) {
    epoll->updateChannel(channel);
}

void EventLoop::delChannel(Channel* channel) {
    epoll->removeChannel(channel);
}

void EventLoop::loop() {
    while(run){
        std::vector<Channel*> channels = epoll->poll(EPOLL_TIMER);
        for(auto & channel : channels){
            handler->HandlerEntry(channel);
        }
    }
}
