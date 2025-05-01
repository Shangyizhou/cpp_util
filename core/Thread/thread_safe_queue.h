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
        if (interrupt_) {
            return;
        }
        queue_.push(value);
        cond_.notify_one();
    }

    /**
     * 若无元素且则阻塞等待
    */
    void Pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] {
            return !this->queue_.empty() || interrupt_;
        });
        value = std::move(queue_.front());
        queue_.pop();

        return;
    }

    /**
     * 非阻塞情况下的Pop
     */
    bool TryPop(T& value) {
        std::unique_lock<std::mutex>(mutex_);
        if (queue_.empty() || interrupt_) {
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

    /**
     * 清除队列元素
     */
    void Clear() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!queue_.empty()) {
            queue_.pop(); 
        }
        cond_.notify_all(); 
    }

    /**
     * 中断队列操作
     * 线程安全情况下设置中断标志位，此时会唤醒阻塞的Pop返回错误
     */
    void Interrupt() {
        std::unique_lock<std::mutex> lock(mutex_);
        interrupt_ = true;
        cond_.notify_all(); 
    }

    /**
     * 恢复队列操作
     * 线程安全情况下设置中断标志位，下一次Push可以继续生效
     */
    void Resume() {
        std::unique_lock<std::mutex> lock(mutex_);
        interrupt_ = false;
    }

    /**
     * 是否处于中断情况
     */
    bool IsInterrupted() const {
        return interrupt_.load(std::memory_order_relaxed);
    }

private:
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue(ThreadSafeQueue&&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;
   
    /**
     * 在const成员函数中修改互斥锁的状态，同时不破坏const成员函数的逻辑语义
     * ​编译错误：非mutable的mutex_在const函数中无法加锁。
    */
    mutable std::mutex mutex_; 
    std::condition_variable cond_;
    std::string name_;
    std::atomic<bool> interrupt_{false}; // 中断标志
    std::queue<T> queue_;
};

    
} // namespace tool
