#pragma once

#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <vector>
#include <memory>
#include <functional>
#include <future>

namespace tool
{
namespace context
{

class ThreadPool {
public:
    ThreadPool(uint32_t size);

    ~ThreadPool();

    bool Start();

    template<typename F, typename... Args>
    void Post(F&&f, Args&&... args) {
        if (stop_.load() || !start_.load()) {
            return;
        }

        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        {
            std::lock_guard<std::mutex> lock(mutex_);
            task_queue_.emplace([task = std::move(task)]{ task(); });
        }

        cond_.notify_all();
    }

    template<typename F, typename... Args>
    auto PostAndGetResult(F&&f, Args&&... args) -> std::shared_ptr<std::future<std::decay_t<std::result_of_t<F(Args...)>>>> {
        if (stop_.load() || !start_.load()) {
            return nullptr;
        }

        using return_type = typename std::decay<std::result_of_t<F(Args...)>>::type;
        auto packaged_task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = packaged_task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            task_queue_.emplace([packaged_task = std::move(packaged_task)]{ (*packaged_task)(); });
        }
        cond_.notify_all();
        
        return std::make_shared<std::future<return_type>>(std::move(res));
    }

private:
    void ThreadEntry();

    void Stop();

private:
    // 线程基本信息的一个封装
    struct ThreadInfo {
        std::shared_ptr<std::thread> thread_ptr_;
        std::thread::id thread_id_;
        std::string name_;

        ThreadInfo(std::shared_ptr<std::thread> thread_ptr, const std::string& name);
        ~ThreadInfo();
    };

    using Task = std::function<void()>;

    std::queue<Task> task_queue_;
    std::vector<std::unique_ptr<ThreadInfo>> thread_list_;
    std::atomic<bool> start_{false};
    std::atomic<bool> stop_{false};
    std::condition_variable cond_;
    mutable std::mutex mutex_;
    uint32_t size_;
};

} // namespace context
} // namespace tool
