#include "include/Channel.h"


Channel::Channel(Socket *_socket, Service _serviceType)
        : socket(_socket),
          fd(_socket->getFd()),
          serviceType(_serviceType),
          inEpoll(false),
          events(EPOLLIN | EPOLLET){}

void Channel::setInEpoll(bool _inEpoll) {
    inEpoll = _inEpoll;
}

int Channel::getFd() const {
    return fd;
}

Service Channel::getServiceType() {
    return serviceType;
}


Channel::~Channel(){
    delete socket;
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
