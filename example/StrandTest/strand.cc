#include "Strand/thread_pool.h"
#include "Strand/executor.h"
#include "Strand/timer_thread.h"
#include <iostream>


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

int main() {
    func1();
    func2();

    return 0;
}