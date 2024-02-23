#include "include/Socket.h"
#include "include/NetAddr.h"
#include "include/Epoll.h"
#include "include/Channel.h"

#define IP_ADDR "0.0.0.0"
#define PORT 8888
#define BUF_SIZE 128

int main() {
    int sock_fd, ret;
    char buf[BUF_SIZE];

    /* create a socket for service listening*/
    auto *listen_socket = new Socket(IP_ADDR, PORT);
    listen_socket->listenOn();

    /* create an epoll instance, and add listen fd */
    auto *ep = new Epoll();
    auto *listen_channel = new Channel(listen_socket, Service::UPPER);
    ep->updateChannel(listen_channel);

    /* create a loop and wait epoll events */
    while (true) {
        std::vector<epoll_event> events = ep->poll(EPOLL_TIMER);
        int fds = events.size();
        for (int i = 0; i < fds; i++) {
            auto ready_channel = (Channel *) events[i].data.ptr;

            /* when a new client connected */
            if (ready_channel->getFd() == listen_socket->getFd()) {
                auto *client_socket = new Socket();
                ret = listen_socket->acceptClient(client_socket->getAddr());
                client_socket->setFd(ret);
                client_socket->printClientConnect();
                client_socket->setUnblock();

                auto *client_channel = new Channel(client_socket, Service::UPPER);
                ep->updateChannel(client_channel);
            }
                /* when received the message from the client */
            else if (events[i].events & EPOLLIN) {
                sock_fd = ready_channel->getFd();
                /* recv() - receive data over a socket. */
                ret = recv(sock_fd, buf, BUF_SIZE, 0);
                serr(ret, "recv()");
                if (ret == 0) {
                    ep->removeChannel(ready_channel);
                    continue;
                }
                std::cout << "    message from client fd " << sock_fd << ": " << buf << std::endl;

                for (int j = 0; j < ret; j++) {
                    *(buf + j) = toupper(*(buf + j));
                }

                /* send() - send data over a socket. */
                send(sock_fd, buf, BUF_SIZE, 0);
                memset(buf, 0, ret);
            }
        }
    }

}
