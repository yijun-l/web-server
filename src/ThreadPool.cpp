#include "include/ThreadPool.h"

ThreadPool::ThreadPool() {
    task_queue = new TaskQueue();
    ret = pthread_mutex_init(&taskQueueMutex, nullptr);
    terr(ret, "pthread_mutex_init()");
    ret = pthread_cond_init(&notEmpty, nullptr);
    terr(ret, "pthread_cond_init()");

}

ThreadPool::~ThreadPool() {
    delete task_queue;
    ret = pthread_mutex_destroy(&taskQueueMutex);
    terr(ret, "pthread_mutex_destroy()");
    ret = pthread_cond_destroy(&notEmpty);
    terr(ret, "pthread_cond_destroy()");
}

void ThreadPool::pushTask(Callback func, void* arg) {
    pthread_mutex_lock(&taskQueueMutex);
    Task tmp_task{func, arg};
    task_queue->getTasks()->push(tmp_task);
    pthread_mutex_unlock(&taskQueueMutex);
}

Task ThreadPool::popTask() {
//    pthread_mutex_lock(&taskQueueMutex);
    Task tmp_task = task_queue->getTasks()->front();
    task_queue->getTasks()->pop();
//    pthread_mutex_unlock(&taskQueueMutex);
    return tmp_task;
}




pthread_mutex_t *ThreadPool::getTaskQueueMutex() {
    return &taskQueueMutex;
}

pthread_cond_t *ThreadPool::getNotEmptyCond() {
    return &notEmpty;
}

bool ThreadPool::isEmpty() {
    return task_queue->getTasks()->empty();
}


