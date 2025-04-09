//
// Created by erkan on 1.04.2025.
//

#include "TaskQueue.h"

void TaskQueue::AddTask(std::function<void()>&& callback) {
    std::lock_guard<std::mutex> lock(mutex);
    tasks.push(std::move(callback));
    ++remainingTasks;
}

void TaskQueue::GetTask(std::function<void()>& task) {
    std::lock_guard<std::mutex> lock(mutex);
    if (tasks.empty()) {
        return;
    }
    task = std::move(tasks.front());
    tasks.pop();
}

void TaskQueue::WaitUntilDone() {
    while (remainingTasks > 0) {
        std::this_thread::yield();
    }
}

void TaskQueue::CompleteTask() {
    --remainingTasks;
}



