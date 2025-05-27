//
// Created by 25031 on 25-5-26.
//

#pragma once

#include "thread_pool.h"

#include <unordered_set>
#include <chrono>
#include <functional>

#include "executor.h"

namespace tool {
namespace context {

using TimerTask = std::function<void(void)>;
using RepeatedTaskId = uint64_t;

class TimerThread {
public:
    struct TimerInterval {
        std::chrono::time_point<std::chrono::high_resolution_clock> interval; // Interval in milliseconds
        TimerTask task;
        int repeated_task_id;

        bool operator<(const TimerInterval& rhs) const { return rhs.interval < interval; }
    };

    TimerThread();

    ~TimerThread();

    bool Start();

    // 简单实现，周期性的定时任务不考虑返回future

    void PostDelayed(Task task, const std::chrono::microseconds& delay);

    // 简单实现，周期性的定时任务不考虑返回future
    // immediate_first 表示是否立即执行第一次任务
   void PostDelayedRepeated(Task task, const std::chrono::microseconds& delay, int num = 1, bool immediate_first = true);


private:
    void ThreadEntry();
    void Stop();
    RepeatedTaskId GetNextRepeatedTaskId() { return repeated_task_id_++; }
    void PostDelayedRepeatedInterval(Task task, const std::chrono::microseconds& delay, RepeatedTaskId id, int num);


    RepeatedTaskId repeated_task_id_ = 0;
    std::unique_ptr<ThreadPool> pool_;
    std::priority_queue<TimerInterval> timer_queue_;
    std::unordered_set<RepeatedTaskId> repeated_task_dict_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic<bool> stop_{false};
    std::atomic<bool> start_{false};
};


} // context
} // tool

