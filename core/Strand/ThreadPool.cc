#include "ThreadPool.h"

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

void ThreadPool::Start() {
    if (start_.load()) {
        return ;
    }

    start_.store(true);
    for (int i = 0; i < size_; i++) {
        thread_list_.emplace_back(std::make_unique<ThreadInfo>(std::make_shared<std::thread>(std::bind(&ThreadPool::ThreadEntry, this)), std::to_string(i)));
    }
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
            task = std::move(task_queue_.front());
            task_queue_.pop_front();
        }
        task();
    }
}

}
}