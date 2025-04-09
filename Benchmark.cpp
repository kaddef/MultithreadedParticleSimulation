//
// Created by erkan on 4.04.2025.
//

#include "Benchmark.h"

#include <iostream>

Benchmark::Benchmark(std::string jobName) {
    startTimePoint = std::chrono::high_resolution_clock::now();
    this->jobName = jobName;
}

Benchmark::~Benchmark() {
    Stop();
}

void Benchmark::Stop() {
    auto endTimePoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

    auto duration = end - start;
    double ms = duration * 0.001;
    std::cout << jobName << " elapsed time: " << duration << "us" << "(" << ms << "ms)" << std::endl;
}

