#pragma once
#include <iostream>
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

class Server {
public:
    Server(const std::string& ip, uint16_t port);
    ~Server();
    void run();

private:
    /* ventLoop* eventLoop */
    Channel* listen_channel;
    EventLoop* event_loop;
};