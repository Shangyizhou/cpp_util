//
// Created by 25031 on 25-5-25.
//

#include "executor.h"

namespace tool {
namespace context {

TaskRunnerTag Executor::AddTaskRunner(const TaskRunnerTag& tag) {
    std::lock_guard<std::mutex> lock(mutex_);
    TaskRunnerTag latest_tag = tag;
    while (task_runner_dict_.find(latest_tag) != task_runner_dict_.end()) {
        latest_tag = GetNextRunnerTag();
    }
    TaskRunnerPtr runner = std::make_unique<ThreadPool>(1);
    runner->Start();
    task_runner_dict_.emplace(latest_tag, std::move(runner));
    return latest_tag;
}

TaskRunnerTag Executor::GetNextRunnerTag() {
    static uint64_t index = 0;
    ++index;
    return index;
}

} // context
} // tool