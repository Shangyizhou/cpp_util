#include "thread_pool.h"

namespace tool {

ThreadPool::ThreadPool() : stop_(true), count_(0) {}

ThreadPool::ThreadPool(const int& count) : stop_(true), count_(count) {}

ThreadPool::~ThreadPool() {
    Stop();
    for (int i = 0; i < count_; i++) {
        thread_list_[i].join();
    }
}

void ThreadPool::Start() {
    std::lock_guard<std::mutex> lock(mutex_);
    stop_ = false;

    if (count_ == 0) {
        count_ = std::thread::hardware_concurrency();
        std::cout << "hardware_concurrency count = " << count_ << std::endl;
    }

    for (int i = 0; i < count_; i++) {
        thread_list_.emplace_back(std::thread(&ThreadPool::ThreadEntry, this));
    }

}

void ThreadPool::Stop() {
    std::lock_guard<std::mutex> lock(mutex_);
    stop_ = true;
    cond_.notify_all(); 
}

void ThreadPool::Enqueue(WorkFunc work_func) {
    std::lock_guard<std::mutex> lock(mutex_);
    task_list_.push(work_func);
    cond_.notify_one();
}

void ThreadPool::ThreadEntry() {
    WorkFunc work;

    while (true) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            while (task_list_.empty()) {
                if (stop_) {
                    return;
                }
                cond_.wait(lock);
            }
            work = task_list_.front();
            task_list_.pop();
        }

        if (work) {
            work();
        } else {
            std::cout << "work function is empty" << std::endl;
        }
    }
}

}