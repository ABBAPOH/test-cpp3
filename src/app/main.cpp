#include "clientthreadpool.h"

#include <iostream>

std::mutex *globalMutex = 0;

class MyTask : public Task
{
public:
    explicit MyTask(int taskId, int clientId) : _taskId(taskId), _clientId(clientId) {}

    void run()
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(10000000));
        {
            // lock cout to correctly order messages
            std::unique_lock<std::mutex> lock(*globalMutex);
            std::cout << "hello from client " << _clientId
                      << " task " << _taskId
                      << " thread " << std::this_thread::get_id()
                      << std::endl;
        }
    }

private:
    int _taskId;
    int _clientId;
};

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::mutex mutex;
    globalMutex = &mutex;

    static int clientCount = 10;
    static int taskCount = 10;
    ClientThreadPool pool;
    if (!pool.init(8)) {
        std::cerr << "Cant init pool" << std::endl;
        return 1;
    }

    for (int taskId = 0; taskId < taskCount; ++taskId) {
        for (int clientId = 0; clientId < clientCount; ++clientId) {
            auto task = new MyTask(taskId, clientId);
            pool.enqueue(clientId, task);
        }
    }

    pool.stop();

    return 0;
}
