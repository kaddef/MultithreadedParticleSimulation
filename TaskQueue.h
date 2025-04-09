//
// Created by erkan on 1.04.2025.
//

#ifndef TASKQUEUE_H
#define TASKQUEUE_H
#include <functional>
#include <mutex>
#include <queue>


class TaskQueue {
private:
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::atomic<int> remainingTasks = 0;
public:
    void AddTask(std::function<void()>&& callback);
    void GetTask(std::function<void()>& task);
    void WaitUntilDone();
    void CompleteTask();
};



#endif //TASKQUEUE_H
