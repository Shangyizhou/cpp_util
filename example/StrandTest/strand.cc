#include "Strand/thread_pool.h"
#include "Strand/executor.h"
#include "Strand/timer_thread.h"
#include <iostream>

#include "Strand/context.h"


void func1() {
    tool::context::ThreadPool pool(1);
    pool.Start();

    pool.Post([] { std::cout << "hello world" << std::endl; });

    int a = 1;
    int b = 2;
    auto res = pool.PostAndGetResult([a, b]{ return a + b; });

    if (res) {
        std::cout << "Result: " << res->get() << std::endl;
    }
}

void func2() {
    tool::context::TimerThread timer_thread;
    timer_thread.Start();
    timer_thread.PostDelayed([]{std::cout << "一次性延时任务，两秒后执行" << std::endl;}, std::chrono::seconds(2));
    timer_thread.PostDelayedRepeated([]{std::cout << "定时任务每间隔两秒执行，共两次" << std::endl;}, std::chrono::seconds(2), 2, false);
    std::this_thread::sleep_for(std::chrono::seconds(100));
}

void func3() {
    tool::context::Executor executor;
    tool::context::TaskRunnerTag tag1 = 1;  // 不要用构造函数语法
    tool::context::TaskRunnerTag tag2 = 2;
    executor.AddTaskRunner(tag1);
    executor.AddTaskRunner(tag2);
    auto res = executor.PostAndGetResult(tag1, [] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 42;
    });
    executor.PostAndGetResult(tag1, [] {
        std::cout << "Executor task 2" << std::endl;
    });
    // 另一个线程，所以会先打印
    executor.PostAndGetResult(tag2, [] {
        std::cout << "Executor delayed task" << std::endl;
    });
    // 阻塞等待线程tag1的第一个任务，且阻塞第二个任务
    if (res) {
        int result = res->get();
        std::cout << "Executor task 1 Result: " << result << std::endl;
    }
}

void func4() {
    tool::context::TaskRunnerTag tag1 = 1;  // 不要用构造函数语法
    tool::context::TaskRunnerTag tag2 = 2;
    tool::context::TaskRunnerTag tag3 = 3;
    NEW_TASK_RUNNER(tag1);
    NEW_TASK_RUNNER(tag2);
    auto res = POST_TASK_AND_GET_RESULT(tag1, [] {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        return 42;
    });
    POST_TASK(tag1, [] {
        std::cout << "tag1 Task 2 executed" << std::endl;
    });
    POST_TASK(tag2, [] {
        std::cout << "tag2 Task 1 executed" << std::endl;
    });

    POST_REPEATED_TASK([] {
        std::cout << "tag3 Repeated Task executed" << std::endl;
    }, std::chrono::seconds(1), 2, false);

    if (res) {
        int result = res->get();
        std::cout << "Executor tag1 Task 1 Result: " << result << std::endl;
    }
}

int main() {
    // func1();
    // func2();
    // func3();
    func4();

    return 0;
}