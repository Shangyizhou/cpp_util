#pragma once

#include <chrono>
#include <vector>

namespace tool
{
    class TimerSimple
    {

    public:
        void Start();

        void Stop();

        double GetCurrentTime() const;

        double GetTotalTime() const;

        int GetCount() const;

        double GetAverageTime() const;

        void Reset();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time_;
        std::chrono::duration<double, std::milli> total_time_{0};
        std::chrono::duration<double, std::milli> current_time_{0};
        int count_ = 0;
    };
};

