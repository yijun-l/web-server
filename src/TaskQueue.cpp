#include "include/TaskQueue.h"

TaskQueue::TaskQueue() {
    taskQueue = new std::queue<Task>();
}

TaskQueue::~TaskQueue() {
    delete taskQueue;
}

std::queue<Task> *TaskQueue::getTasks() {
    return taskQueue;
}

unsigned int TaskQueue::taskSize() {
    return taskQueue->size();
}
