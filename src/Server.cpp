#include "include/Server.h"


Server::Server(){
    handler = new Handler();
    event_loop = new EventLoop(handler);
    handler->setEventLoop(event_loop);
}

Server::~Server() {
    delete event_loop;
}

void Server::run() {
    /* Loop start EventLoop.loop() */
    event_loop->loop();
}

void Server::newService(const std::string &ip, uint16_t port) {
    auto *acceptor = new Acceptor(ip, port, event_loop);
    event_loop->addChannel(acceptor->getChannel());
}
