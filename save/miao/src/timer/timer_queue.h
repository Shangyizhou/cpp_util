#include <chrono>
#include <function/function.hpp>
#include <iostream>
#include <priority_queue>
#include <thread>
#include <condition_variable>
#include <unordered_map>

class TimerQueue {
public:
  // 定时器任务结构
  struct TimerTask {
    std::chrono::time_point<std::chrono::high_resolution_clock> time_point_;
    std::function<void()> callback_;
    bool operator<(const TimerTask &b) const {
      return time_point_ > b.time_point_;
    }
  };

  enum class RepeatedIdState { kInit = 0, kRunning = 1, kStop = 2 };

private:
  std::priority_queue<InternalS> queue_;
  bool running_ = false;
  std::mutex mutex_;
  std::condition_variable cond_;

  wzq::ThreadPool thread_pool_;

  std::atomic<int> repeated_func_id_;
  wzq::ThreadSafeMap<int, RepeatedIdState> repeated_id_state_map_;
};
