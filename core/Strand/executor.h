#pragma once

#include "thread_pool.h"
#include "timer_thread.h"

#include <unordered_set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cstdint>  // 确保包含标准头文件

namespace tool {
namespace context {

using Task = std::function<void(void)>;
using TaskRunnerTag = std::uint64_t;

class Executor {
public:
    Executor() = default;

    // 可以 default，STL 容器自动销毁调用元素的析构函数触发 Stop
    ~Executor() = default;

    TaskRunnerTag AddTaskRunner(const TaskRunnerTag& tag);

    template<typename F, typename... Args>
    void Post(const TaskRunnerTag& runner_tag, F&&f, Args&&... args) {
        TaskRunner* task_runner = task_runner_dict_[runner_tag].get();
        task_runner->Post(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template<typename F, typename... Args>
    auto PostAndGetResult(const TaskRunnerTag& runner_tag, F&&f, Args&&... args)
        -> std::shared_ptr<std::future<std::decay_t<std::result_of_t<F(Args...)>>>> {
        TaskRunner* task_runner = task_runner_dict_[runner_tag].get();
        auto ret = task_runner->PostAndGetResult(std::forward<F>(f), std::forward<Args>(args)...);
        return ret;
    }

private:
    using TaskRunner = ThreadPool;
    using TaskRunnerPtr = std::unique_ptr<TaskRunner>;

    TaskRunnerTag GetNextRunnerTag();

    std::mutex mutex_;
    std::unordered_map<TaskRunnerTag, TaskRunnerPtr> task_runner_dict_;
};

} // context
} // tool

