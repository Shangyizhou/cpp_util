#include <iostream>
#include <memory>

template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}

int main()
{
    std::cout << max<int>(1, 2) << std::endl;        // 2
    std::cout << max<double>(1.0, 2.0) << std::endl; // 2
    std::cout << max<char>('a', 'b') << std::endl;   // b

    std::cout << max(1, 2) << std::endl;     // 2
    std::cout << max(1.0, 2.0) << std::endl; // 2
    std::cout << max('a', 'b') << std::endl; // b

    return 0;
}