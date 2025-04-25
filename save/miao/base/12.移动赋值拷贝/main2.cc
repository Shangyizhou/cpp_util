#include <iostream>
#include <algorithm>

class Foo {
public:
    Foo() : arr_(nullptr), size_(0) {
        std::cout << "默认构造函数" << std::endl;
    }

    Foo(int n) : arr_(new int[n]), size_(n) {
        std::cout << "默认构造函数" << std::endl;
    }

    Foo(const Foo& foo) : arr_(new int[foo.size_]), size_(foo.size_) {
        std::cout << "拷贝构造函数" << std::endl;
        std::copy(foo.arr_, foo.arr_ + foo.size_, arr_);
    }

    Foo(Foo&& foo) noexcept {
        std::cout << "移动构造函数" << std::endl;
        arr_ = foo.arr_;
        size_ = foo.size_;

        foo.arr_ = nullptr;
        foo.size_ = 0;
    }

    Foo& operator=(const Foo& foo) {
        std::cout << "拷贝赋值运算符" << std::endl;
        if (&foo == this) {
            std::cout << "自赋值" << std::endl;
            return *this;
        }
        delete[] arr_;
        arr_ = new int[foo.size_];
        size_ = foo.size_;
        std::copy(foo.arr_, foo.arr_ + foo.size_, arr_);

        return *this; // 链式赋值
    }

    Foo& operator=(Foo&& foo) {
        std::cout << "移动赋值运算符" << std::endl;
        if (&foo == this) {
            std::cout << "自赋值" << std::endl;
            return *this;
        }
       
        delete[] arr_;
        arr_ = foo.arr_;
        size_ = foo.size_;
        
        // 资源转移了
        foo.arr_ = nullptr;
        foo.size_ = 0;
        
        return *this; // 链式赋值
    }

    ~Foo() {
        std::cout << "析构函数" << std::endl;
        if (arr_) {
            delete[] arr_;
        }
    }

    void ShowAddress() {
        std::cout << arr_ << std::endl;
    }

private:
    int* arr_;
    int size_;
};

int main() {
    // 默认构造函数
    // 拷贝构造函数
    // 0x55a435b1be70
    // 0x55a435b1c2a0
    std::cout << "============= 拷贝构造函数 =============" << std::endl;
    Foo foo(3);
    Foo foo2(foo);
    foo.ShowAddress();
    foo2.ShowAddress();
    std::cout << "==========================\n" << std::endl;

    // 移动构造函数
    // 0x55a435b1be70
    // 0
    std::cout << "============= 移动构造函数 =============" << std::endl;
    Foo foo3(std::move(foo));
    foo3.ShowAddress();
    foo.ShowAddress();
    std::cout << "==========================" << std::endl;

    // 拷贝赋值
    std::cout << "============= 拷贝赋值 =============" << std::endl;
    Foo foo4;
    foo4 = foo2;
    foo4.ShowAddress();
    foo2.ShowAddress();
    std::cout << "==========================" << std::endl;

    // 移动赋值
    std::cout << "============= 移动赋值 =============" << std::endl;
    Foo foo5;
    foo5 = std::move(foo2);
    foo5.ShowAddress();
    foo2.ShowAddress(); 
    std::cout << "==========================" << std::endl;


    std::cout << "============= 自赋值 =============" << std::endl;
    foo5 = foo5;
    foo5 = std::move(foo5);
    std::cout << "==========================" << std::endl;


    // 析构函数
    // 析构函数
    // 析构函数
    // 析构函数
    // 析构函数
}