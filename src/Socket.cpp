#include "include/Socket.h"

Socket::Socket() : addr(new NetAddr()), fd(-1) {
    memset(addr, 0, sizeof(addr));
}

Socket::Socket(const std::string& ip, uint16_t port) : addr(new NetAddr(ip, port)), fd(-1) {}

Socket::~Socket() {
    delete addr;
}

void Socket::setFd(int fd) {
    this->fd = fd;
}

int Socket::getFd(){
    return fd;
}

NetAddr* Socket::getAddr(){
    return addr;
}

void Socket::listenOn() {
    int ret;

    /* socket() - creates a new socket. */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    serr(fd, "socket()");

    /* bind() - binds a socket to a specific address and port. */
    ret = bind(fd, (struct sockaddr*)&(addr->addr), addr->addr_len);
    serr(ret, "bind()");

    /* listen() - puts a socket in a listening state, waiting for incoming connections. */
    ret = listen(fd, 10);
    serr(ret, "listen()");

    setNonBlocking(fd);    /* add non-blocking to listen fd */
}

int Socket::acceptClient(NetAddr* client_addr){
    /* accept() - accepts an incoming connection and creates a new socket for communication. */
    int client_fd = accept(fd, (struct sockaddr*)&client_addr->addr, &client_addr->addr_len);
    return client_fd;
}

void Socket::printClientConnect(){
    std::cout << "client fd" << fd << "connected from " << inet_ntoa(addr->addr.sin_addr) << ":" << ntohs(addr->addr.sin_port) << std::endl;
//    printf("client fd %d connected from %s:%d\n", acpt_fd, inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
}

void Socket::setUnblock(){
    setNonBlocking(fd);
}