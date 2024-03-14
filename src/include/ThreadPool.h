#pragma once
#include "TaskQueue.h"
#include "util.h"
#include <pthread.h>


class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();
    void pushTask(Callback func, void* arg = nullptr);
    Task popTask();
    pthread_mutex_t* getTaskQueueMutex();
    pthread_cond_t* getNotEmptyCond();
    bool isEmpty();

private:
    TaskQueue* task_queue;
    pthread_mutex_t taskQueueMutex;
    pthread_cond_t notEmpty;
    int ret;
};
