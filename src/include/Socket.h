#pragma once
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
    int getFd();
    void setFd(int fd);
    NetAddr* getAddr();
    void listenOn();
    int acceptClient(NetAddr* client_addr);
    void printClientConnect();
    void setUnblock();
};

