#include "include/EventLoop.h"

EventLoop::EventLoop() : _epoll(new Epoll()), run(true){
    /* create an epoll instance, and add listen fd */
}

EventLoop::~EventLoop() {
    delete _epoll;
}

void EventLoop::addChannel(Channel* channel) {
    _epoll->updateChannel(channel);
}

void EventLoop::loop() {
    while(run){
        std::vector<epoll_event> events = _epoll->poll(EPOLL_TIMER);
        for(int i = 0; i < events.size(); i++){
            auto ready_channel = (Channel*) events[i].data.ptr;
            if (ready_channel->getServiceType() == Service::LISTEN){
                listenHandler(ready_channel);
            }
            else if (ready_channel->getServiceType() == Service::UPPER){
                upperHandler(ready_channel);
            }
        }
    }
}

void EventLoop::listenHandler(Channel* listen_channel) {
    auto *client_socket = new Socket();
    int client_fd = listen_channel->getSocket()->acceptClient(client_socket->getAddr());
    client_socket->setFd(client_fd);
    client_socket->printClientConnect();
    client_socket->setUnblock();

    auto *client_channel = new Channel(client_socket, Service::UPPER);
    _epoll->updateChannel(client_channel);
}

void EventLoop::upperHandler(Channel* upper_channel) {
    int sock_fd = upper_channel->getFd();
    /* recv() - receive data over a socket. */
    int ret = recv(sock_fd, buf, BUF_SIZE, 0);
    serr(ret, "recv()");
    if (ret == 0) {
        _epoll->removeChannel(upper_channel);
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


