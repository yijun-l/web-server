#pragma once
#include <arpa/inet.h>
#include <string>
#include <cstring>

class NetAddr{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;

    /* default constructor */
    NetAddr();
    /* constructor with ip address and port number */
    NetAddr(const std::string& ip, uint16_t port);
    /* destructor */
    ~NetAddr();

    void setAddr(const std::string& ip, uint16_t port);

};
