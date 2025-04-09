//
// Created by erkan on 1.04.2025.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(int numberOfThreads):
    numberOfThreads(numberOfThreads)
{
    threads.reserve(numberOfThreads);
    for (int i = 0; i < numberOfThreads; i++)
        threads.emplace_back(taskQueue, i);
}

ThreadPool::~ThreadPool() {
    for (auto& thread : threads) {
        thread.stop();
    }
    threads.clear();
}

void ThreadPool::Parallel(int num_obj, std::function<void(int start, int end)>&& callback) {
    int sliceSize = num_obj / numberOfThreads;
    for (int i = 0; i < numberOfThreads; i++) {
        int start = i * sliceSize;
        int end = start + sliceSize;
        taskQueue.AddTask([start, end, &callback](){ callback(start, end);});
    }
    if (sliceSize * numberOfThreads < num_obj) {
        int start = sliceSize * numberOfThreads;
        callback(start, num_obj);
    }
    taskQueue.WaitUntilDone();
}