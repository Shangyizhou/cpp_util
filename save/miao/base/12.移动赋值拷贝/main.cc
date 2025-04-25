// 题目1：拷贝构造函数

// 要求：定义一个MyClass类，包含一个动态分配的整型数组作为成员。实现拷贝构造函数，确保深拷贝发生。

// 验收标准：
// 1.拷贝构造函数应正确复制动态分配的数组。
// 2.测试显示两个对象具有独立的数组副本。
#include <iostream>
#include <algorithm>

#include <iostream>
#include <algorithm> // for std::copy

class MyClass {
public:
    // 默认构造函数
    MyClass() : arr_(nullptr), size_(0) {
        std::cout << "默认构造函数" << std::endl;
    }

    // 带参数的构造函数
    MyClass(int size) : size_(size), arr_(new int[size]) {
        std::cout << "带参数的构造函数" << std::endl;
    }

    // 拷贝构造函数（深拷贝）
    MyClass(const MyClass& other) : size_(other.size_), arr_(new int[other.size_]) {
        std::cout << "拷贝构造函数" << std::endl;
        std::copy(other.arr_, other.arr_ + size_, arr_); // 复制数据
    }

    // 移动构造函数
    MyClass(MyClass&& other) noexcept 
        : arr_(other.arr_), size_(other.size_) {
        std::cout << "移动构造函数" << std::endl;
        other.arr_ = nullptr; // 确保原对象不再拥有资源
        other.size_ = 0;
    }

    // 拷贝赋值运算符
    MyClass& operator=(const MyClass& other) {
        std::cout << "拷贝赋值运算符" << std::endl;
        if (this != &other) { // 自赋值检查
            delete[] arr_;    // 释放现有资源
            
            size_ = other.size_;
            arr_ = new int[size_];
            std::copy(other.arr_, other.arr_ + size_, arr_);
        }
        return *this;
    }

    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept {
        std::cout << "移动赋值运算符" << std::endl;
        if (this != &other) { // 自赋值检查
            delete[] arr_;    // 释放现有资源
            
            arr_ = other.arr_;
            size_ = other.size_;
            
            other.arr_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    ~MyClass() {
        std::cout << "析构函数" << std::endl;
        delete[] arr_;
    }

    void printAddress() const {
        std::cout << "数组地址: " << static_cast<void*>(arr_) << std::endl;
    }

private:
    int* arr_;
    int size_;
};

int main() {
    // 测试1：基本构造
    MyClass obj1(5);
    std::cout << "obj1: ";
    obj1.printAddress();

    // 测试2：拷贝构造（验证深拷贝）
    MyClass obj2(obj1);
    std::cout << "obj2: ";
    obj2.printAddress();

    // 测试3：移动构造
    MyClass obj3(std::move(obj1));
    std::cout << "obj3: ";
    obj3.printAddress();
    std::cout << "移动后的obj1: ";
    obj1.printAddress(); // 应该显示nullptr

    // 测试4：赋值运算符
    MyClass obj4;
    obj4 = obj2;
    std::cout << "obj4: ";
    obj4.printAddress();

    // 测试5：移动赋值
    MyClass obj5;
    obj5 = std::move(obj2);
    std::cout << "obj5: ";
    obj5.printAddress();
    std::cout << "移动后的obj2: ";
    obj2.printAddress(); // 应该显示nullptr
}