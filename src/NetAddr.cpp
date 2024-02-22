#include "include/NetAddr.h"

NetAddr::NetAddr() : addr_len(sizeof(addr)) {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
}

NetAddr::NetAddr(const std::string& ip, uint16_t port) : addr_len(sizeof(addr)) {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton(ip.c_str(), &addr.sin_addr);
    addr.sin_port = htons(port);
}

NetAddr::~NetAddr() {}

void NetAddr::setAddr(const std::string& ip, uint16_t port){
    inet_aton(ip.c_str(), &addr.sin_addr);
    addr.sin_port = htons(port);
}
