#pragma once
#include <queue>

using Callback = void (*)(void*);

struct Task{
    Callback func;
    void* arg;
};

class TaskQueue {
public:
    TaskQueue();
    ~TaskQueue();
    std::queue<Task>* getTasks();
    unsigned int taskSize();

private:
    std::queue<Task>* taskQueue;
};
