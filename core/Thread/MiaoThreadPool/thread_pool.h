#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <string>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>


class ThreadPool {
public:
    explicit ThreadPool(uint32_t thread_count) {
        this->is_available_.store(false);
        this->is_shutdown_.store(false);
        this->thread_count_.store(thread_count);
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool() {
        Stop();
    }

    bool Start();

    bool Stop();

private:
    void AddThread();

private:
    using ThreadPtr = std::shared_ptr<std::thread>;
    using Task = std::function<void()>;

    struct ThreadInfo {
        ThreadInfo() = default;
        ~ThreadInfo();

        ThreadPtr ptr{nullptr};
    };

    using ThreadInfoPtr = std::shared_ptr<ThreadInfo>;

private:
    std::vector<ThreadInfoPtr> work_threads_;
    std::queue<Task> tasks_;

    std::mutex task_mutex_;
    std::condition_variable task_cond_;

    std::atomic<uint32_t> thread_count_{0};
    std::atomic<bool> is_shutdown_{false};
    std::atomic<bool> is_available_{false};
};

ThreadPool::~ThreadPool() {
    Stop();
}

ThreadPool::ThreadInfo::~ThreadInfo() {
    if (ptr && ptr->joinable()) {
        ptr->join();
    }
}

bool ThreadPool::Start() {

}

bool ThreadPool::Stop() {
    if (is_available_.load()) {
        is_shutdown_.store(true);
        task_cond_.notify_all();
        for (auto& thread : work_threads_) {
            if (thread->ptr && thread->ptr->joinable()) {
                thread->ptr->join();
            }
        }
        work_threads_.clear();
        is_available_.store(false);
    }
    return true;
}


void ThreadPool::AddThread() {

}
