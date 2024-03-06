#pragma once
#include <iostream>
#include "Socket.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Handler.h"

class Server {
public:
    Server();
    ~Server();
    void run();
    void newService(const std::string& ip, uint16_t port);

private:
    EventLoop* event_loop;
    Handler* handler;
};