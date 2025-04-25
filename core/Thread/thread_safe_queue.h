#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>

#include "Log/logger.h"

namespace tool
{

/**
 * 可以使用阻塞和非阻塞两种方式
 * try开头就是非阻塞的，不会停留直接返回布尔值
*/
template<typename T>
class ThreadSafeQueue {

public:
    ThreadSafeQueue() {}

    /**
     * 每次push成功都会唤醒一次
    */
    void Push(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        cond_.notify_one();
    }

    /**
     * 若无元素则阻塞等待
    */
    void Pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] {
            return !this->queue_.empty();
        });
        value = std::move(queue_.front());
        queue_.pop();

        return;
    }

    bool TryPop(T& value) {
        std::unique_lock<std::mutex>(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        
        return true;
    }

    bool Empty() {
        std::unique_lock<std::mutex>(mutex_);
        return queue_.empty();
    }

    size_t Size() const {
        std::unique_lock<std::mutex>(mutex_);
        return queue_.size();
    }

private:
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue(ThreadSafeQueue&&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;

    std::queue<T> queue_;
    /**
     * 在const成员函数中修改互斥锁的状态，同时不破坏const成员函数的逻辑语义
     * ​编译错误：非mutable的mutex_在const函数中无法加锁。
    */
    mutable std::mutex mutex_; 
    std::condition_variable cond_;
    std::string name_;
};

    
} // namespace tool
