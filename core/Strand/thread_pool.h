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

    /**
     * @brief 提交任务不提供返回结果
     */
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

    /**
     * @brief 提交任务并获取返回结果，如果任务未完成，T1 会阻塞等待结果。如果结果已经拥有，T2 线程可以直接获取结果不必等待。
     * @return 返回一个shared_ptr的future对象，考虑到会有多个线程共享结果，使用shared_ptr
     */
    template<typename F, typename... Args>
    auto PostAndGetResult(F&&f, Args&&... args) -> std::shared_ptr<std::future<std::decay_t<std::result_of_t<F(Args...)>>>> {
        if (stop_.load() || !start_.load()) {
            return nullptr;
        }

        /**
         * 使用decay消除引用和const修饰符，兼容下面格式的传入
         * PostAndGetResult([](int a, int b) { return a + b; }, 1, 2);
         * PostAndGetResult([](int a, int b) -> int { return a + b; }, 1, 2);
         * PostAndGetResult([](int a, int b) -> int { return a + b; }, std::ref(a), std::ref(b));
         */
        using return_type = typename std::decay<std::result_of_t<F(Args...)>>::type;
        auto packaged_task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = packaged_task->get_future();
        {
            // 使用锁保护任务队列，确保线程安全，因为task_queue_是多个线程共享的资源
            // 使用move语义将packaged_task传递给任务队列，避免不必要的拷贝
            std::lock_guard<std::mutex> lock(mutex_);
            task_queue_.emplace([packaged_task = std::move(packaged_task)]{ (*packaged_task)(); });
        }
        cond_.notify_all();
        
        return std::make_shared<std::future<return_type>>(std::move(res));
    }

private:
    /**
     * @brief 线程入口函数
     */
    void ThreadEntry();

    /**
     * @brief 停止线程池，仅析构函数调用
     */
    void Stop();

private:
    /**
     * @brief 线程基本信息的一个封装，线程池析构直接clear促使析构函数join
     * 为什么 thread_ptr_ 要使用共享指针，我这里想到了一个场景，如果线程池内部有监控线程会访问线程信息，那么就会涉及到多线程访问，适合shared_ptr
     * 试想一下，如果线程池析构时，线程信息还在被监控线程访问，那么如果使用普通指针就会导致悬空指针的问题。或者监控线程访问触发了move语义，导致线程信息被销毁
     */
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
