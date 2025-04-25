#pragma once

#include <chrono>

namespace tool {

class Timer 
{
public:
    void Start();

    void Stop();

    void Reset();

    double GetTotalTime() const;

    double GetAverageTime() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time_;

    std::chrono::duration<double, std::milli> total_time_;
    std::chrono::duration<double, std::milli> current_time_;

    int count_ = 0;;
};


void Timer::Start() {
    start_time_ = std::chrono::high_resolution_clock::now();
}

void Timer::Stop() {
    end_time_ = std::chrono::high_resolution_clock::now();
    current_time_ = end_time_ - start_time_;
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_ - start_time_).count();
    total_time_ += end_time_ - start_time_;
    count_++;
}

void Timer::Reset() {
    start_time_ = end_time_ = {};
    total_time_ = current_time_ = {};
    count_ = 0;
}

double Timer::GetTotalTime() const {
    return total_time_.count();
}

double Timer::GetAverageTime() const {
    if (count_ > 0) {
        return (total_time_ / count_).count();
    }
    return 0.0;
}

}