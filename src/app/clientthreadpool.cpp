#include "clientthreadpool.h"
#include "clientthreadpool_p.h"

#include <assert.h>

ClientRunnable::ClientRunnable(ClientThreadPool *pool) :
    _pool(pool)
{
    setAutoDelete(false);
}

void ClientRunnable::enqueue(Task *task)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.push_back(task);
    _pool->_pool.enqueue(this);
}

void ClientRunnable::run()
{
    std::unique_lock<std::mutex> lock(_mutex);
    assert(!_queue.empty());

    Runnable *runnable = _queue.front();
    _queue.pop_front();
    lock.unlock();

    const bool autoDelete = runnable->autoDelete();
    runnable->run();
    if (autoDelete)
        delete runnable;
}

ClientThreadPool::ClientThreadPool()
{

}

ClientThreadPool::~ClientThreadPool()
{

}

void ClientThreadPool::enqueue(int clientId, Task *task)
{
    auto it = _runnables.find(clientId);
    if (it == _runnables.end()) {
        it = _runnables.emplace(clientId,
                                std::unique_ptr<ClientRunnable>(new ClientRunnable(this))).first;
    }

    auto runnable = it->second.get();
    runnable->enqueue(task);
}
