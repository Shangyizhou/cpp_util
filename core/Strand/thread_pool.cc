#include "thread_pool.h"

namespace tool
{
namespace context
{

ThreadPool::ThreadInfo::ThreadInfo(std::shared_ptr<std::thread> thread_ptr, const std::string& name)
    : thread_ptr_(std::move(thread_ptr)), thread_id_(thread_ptr_->get_id()), name_(std::move(name)) {}

ThreadPool::ThreadInfo::~ThreadInfo() {
    if (thread_ptr_ && thread_ptr_->joinable()) {
        thread_ptr_->join();
    }
}

ThreadPool::ThreadPool(uint32_t size) : size_(size) {}

ThreadPool::~ThreadPool() {
    Stop();
}

bool ThreadPool::Start() {
    if (start_.load()) {
        return true;
    }

    start_.store(true);
    for (int i = 0; i < size_; i++) {
        thread_list_.emplace_back(std::make_unique<ThreadInfo>(std::make_shared<std::thread>(std::bind(&ThreadPool::ThreadEntry, this)), std::to_string(i)));
    }
    return true;
}

void ThreadPool::Stop() {
    if (start_.load()) {
        stop_.store(true);
        cond_.notify_all();
        start_.store(false);
    }
    thread_list_.clear();
}

void ThreadPool::ThreadEntry() {
    while (true) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [this]{ return stop_ || !task_queue_.empty(); });
            if (stop_) {
                break;
            }
            // stop 的时候也会唤醒，此时需要判断任务存在任务
            if (task_queue_.empty()) {
                continue;
            }

            // 经过了上面的判断，这里的move才是安全的
            task = std::move(task_queue_.front());
            task_queue_.pop();
        }
        // 之前前判断task是否可用
        if (task) {
            task();
        }
    }
}

}
}