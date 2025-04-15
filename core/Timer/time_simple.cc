#include "time_simple.h"

namespace tool
{
    void TimerSimple::Start()
    {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    void TimerSimple::Stop()
    {
        end_time_ = std::chrono::high_resolution_clock::now();
        current_time_ = (end_time_ - start_time_);
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>((end_time_ - start_time_)).count();
        total_time_ += (end_time_ - start_time_);
        count_ += 1;
    }

    double TimerSimple::GetCurrentTime() const
    {
        return current_time_.count();
    }

    double TimerSimple::GetTotalTime() const
    {
        return current_time_.count();
    }

    int TimerSimple::GetCount() const
    {
        return count_;
    }

    double TimerSimple::GetAverageTime() const
    {
        if (count_ > 0)
        {
            return (total_time_ / count_).count();
        }
        else
        {
            return 0.0; // 避免除零错误
        }
    }

    void TimerSimple::Reset()
    {
        start_time_ = end_time_ = {};
        total_time_ = current_time_ = {};
        count_ = 0;
    }

} // namespace tool
