#include "timer.h"

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    tool::Timer timer;
    timer.Start();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    timer.Stop();

    // 耗时 5000.1
    std::cout << "耗时 " << timer.GetTotalTime() << std::endl;

    return 0;
}