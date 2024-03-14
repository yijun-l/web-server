#include "include/Handler.h"
#include "include/EventLoop.h"

Handler::Handler(){
    pool = new ThreadPool();
    workers = new pthread_t[WORKER_NUM];
    for(int i = 0; i <WORKER_NUM; i++){
        ret = pthread_create((workers+i), nullptr, workerFunc, (void*)pool);
        terr(ret, "pthread_create()");
    }

};

Handler::~Handler(){
    for(int i = 0; i <WORKER_NUM; i++){
        ret = pthread_cancel(workers[i]);
        terr(ret, "pthread_cancel()");
    }
    delete[] workers;
    delete pool;
}

void Handler::setEventLoop(EventLoop *_event_loop) {
    event_loop = _event_loop;
}
void Handler::HandlerEntry(Channel* channel) {
    auto* arg = new TaskArg{channel, event_loop};
    switch (channel->getServiceType()) {
        case Service::LISTEN:
            pool->pushTask(listenHandler, (void*)arg);
            break;
        case Service::UPPER:

            pool->pushTask(upperHandler, (void*)arg);
            break;
    }
}

void Handler::listenHandler(void* arg) {
    auto* task_arg = (TaskArg*)arg;
    Channel* listen_channel = task_arg->channel;
    EventLoop* event_loop = task_arg->event_loop;
    auto *client_socket = new Socket();
    int client_fd = listen_channel->getSocket()->acceptClient(client_socket->getAddr());
    client_socket->setFd(client_fd);
    client_socket->printClientConnect();
    client_socket->setUnblock();

    auto* client_channel = new Channel(client_socket, Service::UPPER);

    event_loop->addChannel(client_channel);
}

void Handler::upperHandler(void* arg) {
    auto* task_arg = (TaskArg*)arg;
    Channel* upper_channel = task_arg->channel;
    EventLoop* event_loop = task_arg->event_loop;

    int sock_fd = upper_channel->getFd();
    char buf[1024];

    /* recv() - receive data over a socket. */
    int ret = recv(sock_fd, buf, BUF_SIZE, 0);
    serr(ret, "recv()");
    if (ret == 0) {
        event_loop->delChannel(upper_channel);
        return;
    }
    std::cout << "    message from client fd " << sock_fd << ": " << buf << std::endl;

    for (int j = 0; j < ret; j++) {
        *(buf + j) = toupper(*(buf + j));
    }

    /* send() - send data over a socket. */
    send(sock_fd, buf, BUF_SIZE, 0);
    memset(buf, 0, ret);
}

void *Handler::workerFunc(void *arg) {
    auto* pool = (ThreadPool*)arg;
    Task my_task{};
    while(true){
        pthread_mutex_lock(pool->getTaskQueueMutex());
        if (pool->isEmpty()){
            pthread_cond_wait(pool->getNotEmptyCond(), pool->getTaskQueueMutex());
        }
        my_task = pool->popTask();
        pthread_mutex_unlock(pool->getTaskQueueMutex());
        my_task.func(my_task.arg);
        delete (TaskArg*)my_task.arg;
    }

    return nullptr;
}

ThreadPool *Handler::getPool() {
    return pool;
}

