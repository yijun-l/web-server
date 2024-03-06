#include "include/Handler.h"
#include "include/EventLoop.h"

Handler::Handler() = default;

Handler::~Handler() = default;

void Handler::setEventLoop(EventLoop *_event_loop) {
    event_loop = _event_loop;
}
void Handler::HandlerEntry(Channel* channel) {
    switch (channel->getServiceType()) {
        case Service::LISTEN:
            listenHandler(channel);
            break;
        case Service::UPPER:
            upperHandler(channel);
            break;
    }
}

void Handler::listenHandler(Channel* listen_channel) {
    auto *client_socket = new Socket();
    int client_fd = listen_channel->getSocket()->acceptClient(client_socket->getAddr());
    client_socket->setFd(client_fd);
    client_socket->printClientConnect();
    client_socket->setUnblock();

    auto* client_channel = new Channel(client_socket, Service::UPPER);

    event_loop->addChannel(client_channel);
}

void Handler::upperHandler(Channel* upper_channel) {
    int sock_fd = upper_channel->getFd();
    char buf[1024];

    /* recv() - receive data over a socket. */
    int ret = recv(sock_fd, buf, BUF_SIZE, 0);
    serr(ret, "recv()");
    if (ret == 0) {
        event_loop->delChannel(upper_channel);
        return;
    }
    std::cout << "    message from client fd " << sock_fd << ": " << buf << std::endl;

    for (int j = 0; j < ret; j++) {
        *(buf + j) = toupper(*(buf + j));
    }

    /* send() - send data over a socket. */
    send(sock_fd, buf, BUF_SIZE, 0);
    memset(buf, 0, ret);
}


