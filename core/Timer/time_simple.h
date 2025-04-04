#pragma once

#include <chrono>
#include <vector>

namespace tool
{
    class TimerSimple
    {

    public:
        void start();

        void stop();

        double getCurrentTime() const;

        double getTotalTime() const;

        int getCount() const;

        double getAverageTime() const;

        void reset();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time_;
        std::chrono::duration<double, std::milli> total_time_{0};
        std::chrono::duration<double, std::milli> current_time_{0};
        int count_ = 0;
    };
};

