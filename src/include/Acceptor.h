#pragma once
#include <iostream>
#include "EventLoop.h"
#include "Channel.h"

class Acceptor {
private:
    Channel* listen_channel;
    EventLoop* event_loop;

public:
    Acceptor(const std::string &ip, uint16_t port, EventLoop* _event_loop);
    ~Acceptor();
    Channel* getChannel();
};
