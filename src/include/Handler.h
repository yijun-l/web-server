#pragma once
#include "Channel.h"
#include "ThreadPool.h"

#define WORKER_NUM 10

class EventLoop;

struct TaskArg{
    Channel* channel;
    EventLoop* event_loop;
};


class Handler {
public:
    Handler();
    ~Handler();
    void HandlerEntry(Channel* channel);
    void setEventLoop(EventLoop* _event_loop);
    ThreadPool* getPool();
    static void listenHandler(void* arg);
    static void upperHandler(void* arg);

private:

    EventLoop* event_loop{};
    pthread_t* workers;
    int ret;
    static void* workerFunc(void* arg);
    ThreadPool* pool;
};

