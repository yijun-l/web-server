#include "include/Channel.h"

Channel::Channel(Socket *_socket, Service _serviceType)
        : socket(_socket),
          fd(_socket->getFd()),
          serviceType(_serviceType),
          inEpoll(false),
          events(EPOLLIN | EPOLLET){}

Channel::~Channel(){
    delete socket;
}

void Channel::setInEpoll(bool _inEpoll) {
    inEpoll = _inEpoll;
}

int Channel::getFd() const {
    return fd;
}

bool Channel::getInEpoll() const {
    return inEpoll;
}

uint32_t Channel::getEvents() const {
    return events;
}

Socket *Channel::getSocket() {
    return socket;
}

Service Channel::getServiceType() {
    return serviceType;
}

