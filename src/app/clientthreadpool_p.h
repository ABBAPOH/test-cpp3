#ifndef CLIENTTHREADPOOL_P
#define CLIENTTHREADPOOL_P

#include "clientthreadpool.h"

class ClientRunnable : public Runnable
{
public:
    explicit ClientRunnable(ClientThreadPool *pool);

    void enqueue(Task *task);

    void run();

private:
    ClientThreadPool *_pool {nullptr};

    std::mutex _mutex;
    std::deque<Task *> _queue;
};

#endif // CLIENTTHREADPOOL_P

