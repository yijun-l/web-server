#include "include/Acceptor.h"

Acceptor::Acceptor(const std::string &ip, uint16_t port, EventLoop* _event_loop)
    : event_loop(_event_loop){
    auto* listen_socket = new Socket(ip, port);
    listen_socket->listenOn();

    listen_channel = new Channel(listen_socket,Service::LISTEN);

    event_loop->addChannel(listen_channel);
}

Acceptor::~Acceptor() {
    event_loop->delChannel(listen_channel);
    delete listen_channel;
}

Channel *Acceptor::getChannel() {
    return listen_channel;
}
