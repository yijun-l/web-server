#pragma once
#include <sys/epoll.h>
#include <functional>
#include "Socket.h"

enum class Service{
    LISTEN,
    UPPER
};

class Channel {
private:
    Socket* socket;
    int fd;
    Service serviceType;
    bool inEpoll;
    uint32_t events;    /* Epoll events, EPOLLIN|EPOLLET as default */

public:
    Channel(Socket* _socket, Service _serviceType);
    ~Channel();

    void setInEpoll(bool _inEpoll);

    [[nodiscard]] uint32_t getEvents() const;
    [[nodiscard]] int getFd() const;
    [[nodiscard]] bool getInEpoll() const;
    Service getServiceType();
    Socket* getSocket();
};

