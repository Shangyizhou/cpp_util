#include <iostream>

class Foo {
 public:
    Foo() : num_(new int(0)) {}
    const int* GetNum() { return num_; }
    const void ShowNum() { std::cout << *num_ << std::endl; }
    ~Foo() {
        if (num_) {
            delete num_;
        }
    }
 private:
    int* num_;
};

int main() {
    Foo foo;
    foo.ShowNum(); // 0
    const int* p = foo.GetNum();
    int* q = const_cast<int*>(p);
    *q = 10;
    foo.ShowNum(); // 10
}