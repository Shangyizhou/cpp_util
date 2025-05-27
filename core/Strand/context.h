//
// Created by 25031 on 25-5-28.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include "timer_thread.h"
#include "executor.h"

namespace tool {
namespace context {

using TaskRunnerTag = uint64_t;

class ExecutorManager;
class Context {
public:
    Executor* GetExecutor() const;

    TimerThread* GetTimerExecutor() const;

    static Context* GetInstance() {
        static Context* instance = new Context();
        return instance;
    }

    TaskRunnerTag NewTaskRunner(TaskRunnerTag tag);

    Context(const Context& other) = delete;
    Context& operator=(const Context& other) = delete;

private:
    Context();

private:
    std::unique_ptr<ExecutorManager> executor_manager_;
};

} // context
} // tool

#define CONTEXT tool::context::Context::GetInstance()

#define EXECUTOR CONTEXT->GetExecutor()

#define TIMER_EXECUTOR CONTEXT->GetTimerExecutor()

#define NEW_TASK_RUNNER(tag) CONTEXT->NewTaskRunner(tag)

#define POST_TASK(runner_tag, task) EXECUTOR->Post(runner_tag, task)

#define POST_TASK_AND_GET_RESULT(runner_tag, task) EXECUTOR->PostAndGetResult(runner_tag, task)

#define POST_REPEATED_TASK(task, delta, repeat_num, immediate_first) \
TIMER_EXECUTOR->PostDelayedRepeated(task, delta, repeat_num, immediate_first)

#endif //CONTEXT_H
