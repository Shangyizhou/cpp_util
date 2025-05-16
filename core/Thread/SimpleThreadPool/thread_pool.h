#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <string>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace tool
{
    
class ThreadPool {
public:
    using WorkFunc = std::function<void()>;
    ThreadPool();
    ThreadPool(const int& count);
    ~ThreadPool();

    void Start();
    void Stop();
    void Enqueue(WorkFunc work_func);
private:
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(const ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&&) = delete;

    void ThreadEntry();
    std::vector<std::thread> thread_list_;
    std::queue<WorkFunc> task_list_;
    std::atomic<bool> stop_;
    std::atomic<int> count_;

    std::mutex mutex_;
    std::condition_variable cond_;
};

} // namespace tool