//
// Created by erkan on 1.04.2025.
//

#ifndef THREAD_H
#define THREAD_H
#include <functional>
#include <thread>

#include "TaskQueue.h"


class Thread {
private:
    int id;
    std::thread currentThread;
    std::function<void()> task = nullptr;
    bool running = true;
    TaskQueue* taskQueue = nullptr;

public:
    Thread(TaskQueue& taskQueue, int id);
    void run();
    void stop();
};



#endif //THREAD_H
