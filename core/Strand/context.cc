//
// Created by 25031 on 25-5-28.
//

#include "context.h"

namespace tool {
namespace context {

class ExecutorManager {
public:
    ExecutorManager() {
        executor_ = std::make_unique<Executor>();
        timer_executor_ = std::make_unique<TimerThread>();
    }

    ~ExecutorManager() {
        executor_.reset();
        timer_executor_.reset();
    }

    Executor* GetExecutor() const {
        return executor_.get();
    }

    TimerThread* GetTimerExecutor() const {
        return timer_executor_.get();
    }

    TaskRunnerTag NewTaskRunner(TaskRunnerTag tag) const {
        return executor_->AddTaskRunner(tag);
    }

private:
    std::unique_ptr<Executor> executor_;
    std::unique_ptr<TimerThread> timer_executor_;
};

Context::Context() : executor_manager_(std::make_unique<ExecutorManager>()) {}

Executor* Context::GetExecutor() const {
    return executor_manager_->GetExecutor();
}

TimerThread* Context::GetTimerExecutor() const {
    return executor_manager_->GetTimerExecutor();
}

TaskRunnerTag Context::NewTaskRunner(TaskRunnerTag tag) {
    return executor_manager_->NewTaskRunner(tag);
}

} // context
} // tool