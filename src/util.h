#ifndef UTIL_H
#define UTIL_H

#define PORT 8888
#define BUF_SIZE 128
#define MAX_EVENTS 128
#define EPOLL_TIMER 1000

extern void serr(int, const char*);
extern void setNonBlocking (int);

#endif
