#include "Log/logger.h"
#include "Timer/time_simple.h"
#include <vector>
#include <map>
#include <thread>

void long_func() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main() {
    LOG_INFO("Start long_func");
    tool::TimerSimple timer;
    timer.Start();
    long_func();
    timer.Stop();
    LOG_INFO("long_func took {} ms", timer.GetTotalTime());

    return 0;
}