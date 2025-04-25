#include <iostream>

/**
 * 类似递归，需要有一个边界条件
 */
template <typename T>
T sum(T value)
{
    return value;
}

template <typename T, typename... Args>
T sum(T first, Args... args)
{
    return first + sum(args...);
}

int main()
{
    std::cout << sum<int>(1) << std::endl;             // 1
    std::cout << sum<int>(1, 2, 3, 4, 5) << std::endl; // 15
    std::cout << sum<int>(1, 4, 5) << std::endl;       // 10

    return 0;
}