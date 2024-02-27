#include "include/Server.h"


Server::Server(const std::string &ip, uint16_t port) {

    /* create a socket for service listening*/
    auto *listen_socket = new Socket(ip, port);
    listen_socket->listenOn();

    /* create a Channel instance for listening */
    listen_channel = new Channel(listen_socket, Service::LISTEN);

    /* EventLoop* eventLoop = new EventLoop(); */
    event_loop = new EventLoop();

    /* make EventLoop->Epoll add this Channel instance */
    event_loop->addChannel(listen_channel);
}

Server::~Server() {
    delete listen_channel;
    delete event_loop;
}

void Server::run() {
    /* Loop start EventLoop.loop() */
    event_loop->loop();
}
