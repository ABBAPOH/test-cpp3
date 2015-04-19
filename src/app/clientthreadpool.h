#ifndef CLIENTTHREADPOOL_H
#define CLIENTTHREADPOOL_H

#include "threadpool.h"

#include <unordered_map>

class Task : public Runnable
{
    void run() {}
};

class ClientRunnable;

class ClientThreadPool
{
public:
    ClientThreadPool();
    ~ClientThreadPool();

   inline bool init(int threadCount = 1) { return _pool.init(threadCount); }
   inline void stop() { _pool.stop(); }

   void enqueue(int clientId, Task *task);

private:
    ThreadPool _pool;
    std::unordered_map<int, std::unique_ptr<ClientRunnable>> _runnables;

    friend class ClientRunnable;
};

#endif // CLIENTTHREADPOOL_H
