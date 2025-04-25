#include <thread>
#include <vector>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool() = default;
    ThreadPool(int number) : number_(number) {}

    void Run();
private:
    void RunCore();

    int number_;
    atomic<bool> is_run_ {false};
    vector<std::thread> thread_pool_;
};

void ThreadPool::Run() {
    RunCore();
}

void ThreadPool::RunCore() {
    
}

