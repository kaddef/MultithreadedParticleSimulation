//
// Created by erkan on 1.04.2025.
//

#include "Thread.h"

Thread::Thread(TaskQueue &taskQueue, int id):
    id(id),
    taskQueue(&taskQueue)
{
    currentThread = std::thread(&Thread::run, this);
}

void Thread::run() {
    while (running) {
        taskQueue->GetTask(task);
        if (task == nullptr) {
            std::this_thread::yield();
        } else {
            task();
            taskQueue->CompleteTask();
            task = nullptr;
        }
    }
}

void Thread::stop() {
    running = false;
    currentThread.join();
}



