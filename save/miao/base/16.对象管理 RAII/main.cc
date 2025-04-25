#include <iostream>
#include <string>

class ScopedPtr {
public:
    ScopedPtr() {}
   
    ScopedPtr(int* ptr) { ptr_ = ptr; }

    // 移动构造函数
    ScopedPtr(ScopedPtr&& scoped_ptr) {
        ptr_ = scoped_ptr.ptr_;
        scoped_ptr.ptr_ = nullptr;
    }
    
    // 移动赋值运算符
    ScopedPtr& operator=(ScopedPtr&& scoped_ptr) {
        // 自赋值处理
        if (&scoped_ptr == this) {
            return *this;
        }
        // 释放前判空
        if (ptr_) {
            delete ptr_;
        }
        ptr_ = scoped_ptr.ptr_;
        scoped_ptr.ptr_ = nullptr;
        return *this;
    }


    // 3.ScopedPtr应提供一个安全的方式来访问其管理的内存
    int* GetRawPointer() const { return ptr_; }
    
    ~ScopedPtr() {
        if (ptr_) {
            std::cout << "析构函数" << std::endl;
            delete[] ptr_;
        }
    }

private:
    ScopedPtr(const ScopedPtr& scoped_ptr) = delete;
    ScopedPtr& operator=(const ScopedPtr& scoped_ptr) = delete;

    int* ptr_;
};

int main() {
    ScopedPtr ptr = new int[3]{1, 2, 3};

    std::cout << "ptr " << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << *(ptr.GetRawPointer() + i) << " ";
    }
    std::cout << " " << std::endl;


    // ScopedPtr ptr2 = ptr; "ScopedPtr::ScopedPtr(const ScopedPtr &scoped_ptr)" (已声明 所在行数:42) 不可访问C/C++(330)

    ScopedPtr ptr2 = std::move(ptr);

    std::cout << "ptr 2" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << *(ptr2.GetRawPointer() + i) << " ";
    }
    std::cout << " " << std::endl;

    if (!ptr.GetRawPointer()) {
        std::cout << "ptr 被置为空" << std::endl;
    }
    

    return 0;
}