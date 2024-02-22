#include "include/Socket.h"
#include "include/NetAddr.h"
#include "include/Epoll.h"

#define IP_ADDR "0.0.0.0"
#define PORT 8888
#define BUF_SIZE 128

int main(int argc, char* argv[]){
    int sock_fd, ret;
    char buf[BUF_SIZE];
    Socket* fd_set[1024] = {nullptr};

    Socket* lisn_fd = new Socket(IP_ADDR, PORT);
    lisn_fd->listenOn();

    Epoll* ep = new Epoll();
    ep->addFd(lisn_fd->getFd(), EPOLLIN | EPOLLET);

    while(1){
        std::vector<epoll_event> events = ep->poll();
        int fds = events.size();
        for(int i = 0; i < fds; i++) {
            if (events[i].data.fd == lisn_fd->getFd()) {
                Socket *client_sock = new Socket();
                ret = lisn_fd->acceptClient(client_sock->getAddr());
                client_sock->setFd(ret);
                fd_set[ret] = client_sock;
                client_sock->printClientConnect();
                client_sock->setUnblock();
                ep->addFd(client_sock->getFd(), EPOLLIN | EPOLLET);
            } else if (events[i].events & EPOLLIN) {
                sock_fd = events[i].data.fd;
                /* recv() - receive data over a socket. */
                ret = recv(sock_fd, buf, BUF_SIZE, 0);
                serr(ret, "recv()");
                if (ret == 0){
                    ep->delFd(sock_fd);
                    std::cout << "client fd " << sock_fd << " dropped" << std::endl;
                    close(sock_fd);
                    delete fd_set[sock_fd];
                    fd_set[sock_fd] = nullptr;
                    continue;
                }
                std::cout << "    message from client fd " << sock_fd << ": " << buf <<std::endl;

                for (int i = 0; i < ret; i++)
                {
                    *(buf + i) = toupper(*(buf + i));
                }

                /* send() - send data over a socket. */
                send(sock_fd, buf, BUF_SIZE, 0);
                memset(buf, 0, ret);
            }
        }
    }

    delete lisn_fd;

    return 0;
}
