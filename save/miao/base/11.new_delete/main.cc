// new delete new[] delete[]的使用和理解

// 题目1：单个对象的动态内存分配与释放

// 编写一个C++程序，定义一个简单的类（例如，Point类，包含x和y两个整型成员变量），使用new操作符动态分配一个Point对象，并通过指针访问其成员变量，最后使用delete操作符释放该对象。

// 验收标准：
// 1.程序应能正确创建并初始化一个Point对象。
// 2.应能通过指针正确访问并修改Point对象的成员变量。
// 3.应能正确释放分配给Point对象的内存，避免内存泄漏。

// 题目2：对象数组的动态内存分配与释放

// 扩展题目1中的Point类，编写一个C++程序，使用new[]操作符动态分配一个包含5个Point对象的数组，并通过指针数组遍历并打印每个Point对象的x和y值，最后使用delete[]操作符释放该数组。

// 验收标准：
// 1.程序应能正确创建并初始化一个包含5个Point对象的数组。
// 2.应能通过指针数组正确访问并打印每个Point对象的成员变量。
// 3.应能正确释放分配给Point对象数组的内存，避免内存泄漏。

// 题目3：异常处理

// 修改题目2中的程序，增加异常处理机制，以便在new[]分配内存失败时捕获std::bad_alloc异常，并打印一条错误消息。

// 验收标准：
// 1.程序应能捕获std::bad_alloc异常。
// 2.当内存分配失败时，应能打印出相应的错误消息。
// 3.其余部分应符合题目2的验收标准。
#include <iostream>
#include <exception>
#include <stdexcept>

class Point {
public:
    Point() = default;
    Point(int x, int y) : x_(x), y_(y) {}
    int GetX() const { return x_; }
    int GetY() const { return y_; }

    friend std::ostream& operator<<(std::ostream& os, const Point& point);

private:
    int x_;
    int y_;
};

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "point.x = " << point.x_ << " point.y = " << point.y_;
}

void func1() {
    Point* point = new Point(1, 2);
    std::cout << *point << std::endl;
    delete point;
}

void func2() {
    constexpr int size = 5;
    Point* point_list = new Point[size] {
        {0,0},
        {0,0},
        {0,0},
        {0,0},
        {0,0},
    };
    for (int i = 0; i < size; i++) {
        std::cout << point_list[i] << std::endl;
    }
    delete[] point_list;

    if (point_list == nullptr) {
        std::cout << "point list 为 nullptr " <<  std::endl;
        return;
    }
    std::cout << "point list 不为 nullptr " <<  std::endl;
    point_list = nullptr;
}

void func3() {
    constexpr int64_t size = 1e10;
    Point* point_list = nullptr;
    try {
        point_list = new Point[size];
    } catch (const std::bad_alloc& e) {
        std::cerr << e.what() << std::endl;
        delete[] point_list;
    }
    delete[] point_list;
}

int main() {
    func1();
    std::cout << "------------" << std::endl;
    func2();
    std::cout << "------------" << std::endl;
    func3();
    
    return 0;
}
