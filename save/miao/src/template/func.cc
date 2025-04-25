#include <iostream>
#include <typeinfo>
#include <cxxabi.h> // 用于 demangle 类型名称

template<typename T>
auto findMax(T a, T b) -> decltype(a) {
    return a > b ? a : b;
}

int main() {
    auto res = findMax(1, 2);
    auto res2 = findMax<double>(2.0, -2.0);

    std::cout << res << std::endl;
    std::cout << res2 << std::endl;

    return 0;
}