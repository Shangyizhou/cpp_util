#include <iostream>
#include <vector>

// constexpr 修饰变量，替代宏定义
constexpr int kBufferSize = 1024;
constexpr double kPi = 3.14;

// 使用 constexpr 修饰函数
constexpr int GetValue(int a, int b) {
    return a + b;
}

// constexpr 可以直接修饰构造函数，创建编译器可用的对象
struct Point
{
    constexpr Point(int x, int y) : x_(x), y_(y) {}
    constexpr int x() const { return x_; }
    constexpr int y() const {return y_; }

private:
    int x_, y_;
};

constexpr Point kOrigin(0, 0);  // 编译期构造对象
constexpr int kX = kOrigin.x(); // 编译期调用成员函数

int main() {
    // 数组大小使用常量表达式
    int array[kBufferSize];
    std::vector<int> a(GetValue(1, 2));
    std::cout << a.size() << std::endl;

    return 0;
}