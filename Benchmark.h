//
// Created by erkan on 4.04.2025.
//

#ifndef BENCHMARK_H
#define BENCHMARK_H
#include <chrono>


class Benchmark {
private:
    void Stop();
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    std::string jobName;
public:
    Benchmark(std::string jobName);
    ~Benchmark();
};



#endif //BENCHMARK_H
