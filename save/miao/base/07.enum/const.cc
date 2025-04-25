#include <iostream>

// 题目1：const变量的基本使用
// 1.声明一个const整型变量并初始化。
// 2.尝试修改该变量的值，并确保编译器报错。

// 题目2：const指针的使用
// 1.声明一个指向const整数的指针，并通过该指针初始化一个整数。
// 2.尝试通过指针修改其指向的值，确保编译器报错。
// 3.声明一个指针常量，并初始化它指向一个整数。
// 4.尝试修改指针常量的值（即让它指向另一个地址），确保编译器报错。

// 题目3：const修饰函数参数和返回值
// 1.编写一个函数，其参数为const引用，用于输出参数的值但不修改它。
// 2.编写一个函数，其返回值为const指针，指向一个静态或全局变量，确保返回的指针指向的内容不能被修改。

// 题目4：const成员函数
// 1.编写一个类，其中包含至少一个成员变量和一个const成员函数。const成员函数应2.保证不修改类的任何成员变量（除了mutable修饰的变量）。
// 3.在main函数中创建该类的对象，并调用const成员函数，验证其不会修改对象的状态。

void func1() {
    // error: assignment of read-only variable ‘a’
    // const int a = 1;
    // a = 0;
}

// void func2() {
//     int *const p = nullptr;
//     *p = 1;

//     int b = 2;
//     p = &b; // 表达式必须是可修改的左值C/C++(137)

//     const int* q = nullptr;
//     q = &b;
//     *q = 2; // 表达式必须是可修改的左值C/C++(137)
// }

static int global = 1;

void func3_1(const int& a) {
    std::cout << "a " << a << std::endl;
}

const int* func3_2() {
    const int*p = &global;
    return p;
}

// 题目4：const成员函数
// 1.编写一个类，其中包含至少一个成员变量和一个const成员函数。const成员函数应2.保证不修改类的任何成员变量（除了mutable修饰的变量）。
// 3.在main函数中创建该类的对象，并调用const成员函数，验证其不会修改对象的状态。

class Foo {
public:
    // Foo() = default;  不能  必须显式定义构造函数并初始化 const 成员,或者 const 对象有默认值
    Foo() : a_(0), b_(0) {}
    explicit Foo(int a) : a_(a), b_(0) {}

    const int GetNum() { return a_; }

private:
    int a_;
    const int b_;
};

int main() {
    // const int* p = func3_2();
    // *p = 2; // 表达式必须是可修改的左值C/C++(137)

    Foo foo;
    std::cout << foo.GetNum() << std::endl;


}