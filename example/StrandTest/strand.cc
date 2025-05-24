#include "Strand/ThreadPool.h"

#include <iostream>

int main() {
    tool::context::ThreadPool pool(1);
    pool.Start();

    pool.Post([] { std::cout << "hello world" << std::endl; });

    int a = 1;
    int b = 2;
    auto res = pool.PostAndGetResult([a, b]{ return a + b; });

    if (res) {
        std::cout << "Result: " << res->get() << std::endl;
    }

    return 0;
}