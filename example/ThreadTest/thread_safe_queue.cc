#include "Thread/thread_safe_queue.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <iomanip>

// 1. 基本数据类型测试
void TestPrimitiveType() {
    tool::ThreadSafeQueue<int> queue;
    
    // 生产者线程
    std::thread producer([&]{
        for (int i = 0; i < 10; ++i) {
            queue.Push(i);
            std::cout << "Produced: " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // 消费者线程
    std::thread consumer([&]{
        for (int i = 0; i < 10; ++i) {
            int value;
            queue.Pop(value);
            std::cout << "Consumed: " << value << std::endl;
        }
    });

    producer.join();
    consumer.join();
}

// 2. 智能指针测试
void TestSmartPointer() {
    tool::ThreadSafeQueue<std::shared_ptr<std::string>> queue;

    auto producer = [&]{
        for (int i = 0; i < 5; ++i) {
            auto data = std::make_shared<std::string>("Msg-" + std::to_string(i));
            queue.Push(data);
            std::cout << "Produced: " << *data << std::endl;
        }
    };

    auto consumer = [&]{
        for (int i = 0; i < 5; ++i) {
            std::shared_ptr<std::string> data;
            queue.Pop(data);
            std::cout << "Consumed: " << *data << std::endl;
        }
    };

    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
}



// 性能测试工具
class PerfTimer {
public:
    void Start() { start_ = std::chrono::high_resolution_clock::now(); }
    double Stop() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start_).count();
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

// 3. 多线程吞吐量测试
void TestThroughput() {
    tool::ThreadSafeQueue<int> queue;
    std::atomic<int> counter{0};
    const int kTotalOps = 1000000;
    const int kProducerThreads = 4;
    const int kConsumerThreads = 4;

    PerfTimer timer;
    
    // 生产者线程组
    std::vector<std::thread> producers;
    for (int i = 0; i < kProducerThreads; ++i) {
        producers.emplace_back([&]{
            while (counter.fetch_add(1, std::memory_order_relaxed) < kTotalOps) {
                queue.Push(1);
            }
        });
    }

    // 消费者线程组
    std::vector<std::thread> consumers;
    for (int i = 0; i < kConsumerThreads; ++i) {
        consumers.emplace_back([&]{
            int value;
            while (counter.load(std::memory_order_relaxed) < kTotalOps || !queue.Empty()) {
                if (queue.TryPop(value)) {
                    // 模拟处理
                    std::atomic_signal_fence(std::memory_order_acq_rel);
                }
            }
        });
    }

    timer.Start();
    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();
    double elapsed = timer.Stop();

    std::cout << "\n=== Throughput Test ==="
              << "\nThreads: " << kProducerThreads << " producers + " 
              << kConsumerThreads << " consumers"
              << "\nOperations: " << kTotalOps
              << "\nElapsed: " << std::fixed << std::setprecision(3) << elapsed << "s"
              << "\nOps/sec: " << static_cast<int>(kTotalOps / elapsed) 
              << std::endl;
}

// 4. 竞争条件测试
void TestContention() {
    tool::ThreadSafeQueue<int> queue;
    const int kThreads = 8;
    const int kOpsPerThread = 100000;
    std::atomic<int> completed{0};

    auto worker = [&](int id) {
        for (int i = 0; i < kOpsPerThread; ++i) {
            if (id % 2 == 0) {
                queue.Push(i);
            } else {
                int val;
                queue.Pop(val);
            }
        }
        completed.fetch_add(1);
    };

    PerfTimer timer;
    std::vector<std::thread> threads;
    for (int i = 0; i < kThreads; ++i) {
        threads.emplace_back(worker, i);
    }

    timer.Start();
    while (completed.load() < kThreads) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    double elapsed = timer.Stop();

    std::cout << "\n=== Contention Test ==="
              << "\nThreads: " << kThreads
              << "\nOperations/thread: " << kOpsPerThread
              << "\nElapsed: " << elapsed << "s"
              << "\nTotal ops: " << (kThreads * kOpsPerThread)
              << "\nOps/sec: " << static_cast<int>((kThreads * kOpsPerThread) / elapsed)
              << std::endl;
}

int main() {
    std::cout << "==== Basic Test ====\n";
    TestPrimitiveType();

    std::cout << "\n==== Smart Pointer Test ====\n";
    TestSmartPointer();

    // std::cout << "\n==== Performance Tests ====\n";
    // TestThroughput();
    // TestContention();

    return 0;
}