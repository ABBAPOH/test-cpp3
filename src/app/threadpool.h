#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "runnable.h"
#include <condition_variable>
#include <deque>
#include <thread>
#include <vector>

class ThreadPool
{
public:    
    ThreadPool();
    ~ThreadPool();

    bool init(int threadCount = 1);
    void stop();

    void enqueue(Runnable *runnable);

private:
    void run();

private:
    std::vector<std::thread> _threads;
    int _threadCount {1};
    bool _running {false};
    std::mutex _mutex;
    std::deque<Runnable *> _queue;
    std::condition_variable _condition;
    std::condition_variable _wait_for_done_condition;
};

#endif // THREADPOOL_H
