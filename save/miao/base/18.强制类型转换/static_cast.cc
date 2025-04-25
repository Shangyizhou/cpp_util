#include <iostream>
#include <typeinfo>  // 用于typeid

class Base {
public:
    Base() : base_value(10) {}
    virtual ~Base() {}  // 必须要有虚函数才能使用dynamic_cast
    
    int base_value = 10;  // 共有的成员变量
};

class Derived : public Base {
public:
    Derived() : derived_value(20) {}
    
    int derived_value = 20;  // 派生类特有的成员变量
};

int main() {
    Derived derived;

    Base* base = dynamic_cast<Base*>(&derived);
    std::cout << "访问共有对象" << base->base_value << std::endl;

    Derived* derived2 = static_cast<Derived*>(base);
    std::cout << "访问派生类对象" << derived2->derived_value << std::endl;

    // 这样不安全，比如如下的情况
    // 向下转换都是不安全的，可能这个 Base 实际指向的并不是派生类对象，所以需要 dynamic_cast 
    Base base2;
    Derived* derived3 = static_cast<Derived*>(&base2);
    std::cout << "可以通过编译，尝试访问派生类对象" << std::endl;

    return 0;
}