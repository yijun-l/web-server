#pragma once
#include <unistd.h>
#include <iostream>
#include "NetAddr.h"
#include "util.h"

class Socket{
private:
    int fd;
    NetAddr* addr;

public:
    Socket();
    Socket(const std::string& ip, uint16_t port);
    ~Socket();
    [[nodiscard]] int getFd() const;
    void setFd(int p_fd);
    NetAddr* getAddr();
    void listenOn();
    int acceptClient(NetAddr* client_addr) const;
    void printClientConnect();
    void setUnblock() const;
};

