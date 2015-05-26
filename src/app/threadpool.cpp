#include "threadpool.h"

#include <assert.h>
#include <iostream>

/*!
    \class ThreadPool
    Simle thread pool class that can run a single runnable
*/

ThreadPool::ThreadPool()
{
}

ThreadPool::~ThreadPool()
{
    assert(!_running);
}

void ThreadPool::enqueue(Runnable *runnable)
{
    if (!runnable)
        return;

    std::unique_lock<std::mutex> lock(_mutex);
    if (!_running)
        return;

    _queue.push_back(runnable);
    _condition.notify_all();
}

bool ThreadPool::init(int threadCount)
{
    if (threadCount <= 0) {
        std::cerr << "Invalid thread count number: " << threadCount << std::endl;
        return false;
    }

    if (_running) {
        std::cerr << "Pool is already initialized" << std::endl;
        return false;
    }

    _threadCount = threadCount;
    _running = true;
    for (int i = 0; i < _threadCount; ++i) {
        _threads.push_back(std::thread([this]()
        {
            run();
        } ));
    }

    return true;
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_queue.empty())
            _wait_for_done_condition.wait(lock);
        _running = false;
        _condition.notify_all();
    }
    for (std::thread &thread: _threads)
        thread.join();
    _threads.clear();
    assert(_threads.empty());
}

void ThreadPool::run()
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (_running) {
        if (!_queue.empty()) {
            Runnable *runnable = _queue.front();
            _queue.pop_front();

            lock.unlock();
            const bool autoDelete = runnable->autoDelete();
            runnable->run();
            if (autoDelete)
                delete runnable;
            lock.lock();
        } else {
            _wait_for_done_condition.notify_all();
            _condition.wait(lock);
        }
    }
}
