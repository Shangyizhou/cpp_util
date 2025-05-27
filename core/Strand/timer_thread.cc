//
// Created by 25031 on 25-5-26.
//

#include "timer_thread.h"
#include <iostream>
namespace tool {
namespace context {
TimerThread::TimerThread() : pool_(std::make_unique<ThreadPool>(1)) {}

TimerThread::~TimerThread() {
    Stop();
}

bool TimerThread::Start() {
    if (!pool_) {
        return false;
    }
    bool ret = pool_->Start();
    start_.store(true);
    pool_->Post(&TimerThread::ThreadEntry, this);
    return ret;
}

// 注意，TimerThread 析构时候需要等待线程池执行完毕，否则无人唤醒阻塞的游离线程
// pool_ 在析构时会等待所有任务完成，完成阻塞等待
void TimerThread::Stop() {
    start_.store(false);
    stop_.store(true);
    cond_.notify_all();
    pool_.reset();
}

void TimerThread::ThreadEntry() {
    while (true) {
        TimerInterval timer_node;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [this]{ return stop_ || !timer_queue_.empty(); });
            if (stop_) {
                break;
            }

            if (timer_queue_.empty()) {
                continue;
            }

            timer_node = timer_queue_.top();
            auto now = std::chrono::high_resolution_clock::now();
            auto diff = timer_node.interval - now; // 预期时间与当前时间差
            // 记得转换
            if (std::chrono::duration_cast<std::chrono::microseconds>(diff).count() > 0) {
                // 预期时间还未到，等待剩余时间
                cond_.wait_for(lock, diff);
                continue;
            } else {
                // 预期时间到了，获取任务
                timer_node = std::move(timer_node);
                timer_queue_.pop();
            }
        }
        if (timer_node.task) {
            timer_node.task();
        }
    }
}

void TimerThread::PostDelayed(Task task, const std::chrono::microseconds& delay) {
    if (stop_.load() || !start_.load()) {
        return;
    }

    auto time_node = TimerInterval();
    time_node.task = std::move(task);
    time_node.repeated_task_id = GetNextRepeatedTaskId();
    time_node.interval = std::chrono::high_resolution_clock::now() + delay;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        timer_queue_.emplace(std::move(time_node));
    }
    cond_.notify_one();
}

void TimerThread::PostDelayedRepeated(Task task, const std::chrono::microseconds& delay, int num, bool immediate_first) {
    if (stop_.load() || !start_.load()) {
        return;
    }

    auto repeated_task_id = GetNextRepeatedTaskId();
    repeated_task_dict_.insert(repeated_task_id);

    if (immediate_first) {
        // 立即执行第一次
        task();
    }

    PostDelayedRepeatedInterval(std::move(task), delay, repeated_task_id, num);
}

void TimerThread::PostDelayedRepeatedInterval(Task task, const std::chrono::microseconds& delay, RepeatedTaskId task_id, int num) {
    if (stop_.load() || !start_.load() || repeated_task_dict_.find(task_id) == repeated_task_dict_.end() || num == 0) {
        return;
    }

    auto next_task = [this, task = std::move(task), delay, task_id, num]() {
        task();
        PostDelayedRepeatedInterval(std::move(task), delay, task_id, num - 1);
    };

    auto time_node = TimerInterval();
    time_node.task = std::move(next_task);
    time_node.repeated_task_id = task_id;
    time_node.interval = std::chrono::high_resolution_clock::now() + delay;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        timer_queue_.emplace(std::move(time_node));
    }
    // 释放锁再通知唤醒，工作线程被唤醒后，可能因锁未释放而立即阻塞，降低吞吐量，单线程有可能一直阻塞
    cond_.notify_one();
}

} // context
} // tool