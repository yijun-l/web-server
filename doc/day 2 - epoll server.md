# Building a High-Concurrency Server #

We have implemented a simple TCP server in ["day 1 - simple tcp server"](./day%201%20-%20simple%20tcp%20server.md). However, when a Client connects, the Server becomes blocked in ```resv()``` function, which means it can only serve one Client at a time.

To serve multiple clients simultaneously, there are several methods:

+ Multi-Process
+ Multi-Thread
+ I/O Multiplexing (select, poll and epoll)

To construct a high-concurrent server, we typically use **epoll** in a single thread (or multi-threading on a multi-core CPU)

<br>

## epoll ##
In Linux, **epoll** is an event notification mechanism that allows an application to efficiently monitor multiple file descriptors to see if I/O operations can be performed.

The file descriptors monitored by **epoll** can opearte in either **Level-Triggered** (default) or **Edge-Triggered** mode. 

> **Level-Triggered** (LT): the file descriptor triggers an event as long as the event condition is true. If the condition remains true after event processed, the fd will continue to trigger events
<br><br>
> **Edge-Triggered** (ET): the file descriptor triggers an event ONLY when its status changed.

When construct a server with epoll, it's better to set fd to nonblocking (```O_NONBLOCK``` flag) and Edge-Triggered (```EPOLLET``` flag), and completed a I/O event handling only after ```read()``` or ```write()``` return ```EAGAIN```. 

## epoll System Calls ##

**epoll** API provides following system calls to create and manage an **epoll** instance:

### 1. Create an epoll instance ###

```epoll_create()``` creates an **epoll** instance (a file descriptor). The ```size``` argument is ignored since Linux 2.6.8, but its value must be greater than 0.

If the **epoll** instance no longer required, it should be closed by ```close()```.

```c
int epoll_create(int size);
```

### 2. Control an epoll instance ###

```epoll_ctl()``` used to perform specific operation ```op``` (add/modify/delete) on file descriptors ```fd``` in an epoll instance ```epfd```, and the ```fd``` event (we prefer to monitor) pointered by ```event```.

```c
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

typedef union epoll_data {
	void        *ptr;
	int          fd;
	uint32_t     u32;
	uint64_t     u64;
} epoll_data_t;

struct epoll_event {
	uint32_t     events;      /* Epoll events */
	epoll_data_t data;        /* User data variable */
};
```

### 3. Wait for an I/O event ###

Once an epoll instance ```epfd``` properly set, we should allocate a memory area ``` events[maxevents]``` and invoke system call ```epoll_wait()``` to wait for specific events.

When an I/O event happened or timer ```timeout``` expired, ```epoll_wait()``` returns the number of file descriptors ready for the requested I/O, or zero if no file descriptor became ready.

```c
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

