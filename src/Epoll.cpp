#include "include/Epoll.h"

Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd = epoll_create(1);
    serr(epfd, "epoll_create()");
    events = new struct epoll_event[MAX_EVENTS]{};
}

Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

std::vector<Channel*> Epoll::poll(int timeout) {
    std::vector<Channel*> activeEvents;
    int fds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    serr(fds, "epoll_wait()");
    activeEvents.reserve(fds);
    for (int i = 0; i < fds; i++) {
        activeEvents.push_back((Channel*)events[i].data.ptr);
    }
    return activeEvents;
}

void Epoll::updateChannel(Channel *channel) const {
    int ret;
    struct epoll_event ev{};
    ev.events = channel->getEvents();
    ev.data.ptr = channel;

    if (channel->getInEpoll()) {
        ret = epoll_ctl(epfd, EPOLL_CTL_MOD, channel->getFd(), &ev);
        serr(ret, "epoll_ctl() modify fd");
    } else {
        ret = epoll_ctl(epfd, EPOLL_CTL_ADD, channel->getFd(), &ev);
        serr(ret, "epoll_ctl() add fd");
        channel->setInEpoll(true);
    }
}

void Epoll::removeChannel(Channel *channel) const {
    if (channel->getInEpoll()) {
        int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, channel->getFd(), nullptr);
        serr(ret, "epoll_ctl() delete client fd");
        delete channel;
    }
}

