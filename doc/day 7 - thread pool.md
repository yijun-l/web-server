# Thread Pool #

## Relevant System Call ##

### Thread ###

```pthread_self()``` returns the ID of the calling thread.
```c
pthread_t pthread_self(void);
```
<br>

```pthread_create()```  starts a new thread in the calling process. The new thread starts execution by invoking ```start_routine()``` with argument ```arg```.
```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
```
<br>

```pthread_cancel()``` sends a cancellation request to the thread ```thread```.
```c
int pthread_cancel(pthread_t thread);
```
<br>

```pthread_join()```  waits for the thread specified by ```thread``` to terminate.
```c
int pthread_join(pthread_t thread, void **retval);
```
<br>

```pthread_detach()``` marks the thread identified by ```thread``` as detached. When a detached thread terminates, its resources are automatically released back to the system without the need for another thread to join with the terminated thread.
```c
int pthread_detach(pthread_t thread);
```
<br>

```pthread_exit()``` terminates the calling thread and returns a value via ```retval```
```c
void pthread_exit(void *retval);
```

<br>

### Mutex ###

The Mutex (Mutual Exclusion) are low-level primitives used to cooridinate concurrent access to mutable data.  Threads are excluded from accessing data at the same time.

```pthread_mutex_init()```  initialized  an  mutex with attributes specified by attr. If attr is NULL, the default mutex attributes are used.
```c
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;    /* equivalent to pthread_mutex_init(&mutex, NULL); */
```
<br>


```pthread_mutex_destroy()``` destroys the mutex object.
```c
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```
<br>


The mutex shall be locked by calling ```pthread_mutex_lock()```. If the mutex is already locked by another thread, the calling thread shall block until the mutex becomes available.
```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
```
<br>


Same as ```pthread_mutex_lock()```, but ```pthread_mutex_trylock()``` will return immediately if mutex is currently locked (by another thread).
```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```
<br>


```pthread_mutex_unlock()```  releases the mutex.
```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

<br>

###  Condition Variable ###

Condition Variable are synchronization primitives used in multi-threaded programming to manage the flow of execution between threads. They are typically used in conjunction with mutexes to control access to shared resources and coordinate the activities of multiple threads.


```pthread_cond_init()``` initializes the condition variable, using the condition attributes specified in cond_attr, or default attributes if cond_attr is NULL.
```c
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

<br>

```pthread_cond_destroy ()``` destroys a condition variable.
```c
int pthread_cond_destroy(pthread_cond_t *cond);
```

<br>

```pthread_cond_wait()```  atomically unlocks the mutex and waits for the condition variable to be signaled. The thread execution is suspended and does not consume any CPU time until the condition variable is signaled.
```c
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```

<br>

```pthread_cond_timedwait()```  is similar like ```pthread_cond_wait()```, but it blocks until the condition is signaled or until the time of day (spcified by the last argument).
```c
int	pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
```

<br>

```pthread_cond_signal()``` unblocks one thread that is blocked on the condition variable.
```c
int	pthread_cond_signal(pthread_cond_t *cond);
```

<br>

```pthread_cond_broadcast()``` unblocks all threads that are blocked on the condition variable.
```c
int	pthread_cond_broadcast(pthread_cond_t *cond);
```
