#include <iostream>
#include <string>

class StringWrapper {
public:
    StringWrapper() {}

    StringWrapper(const std::string& str) {
        std::cout << "拷贝构造函数" << std::endl;
        str_ = str;
    }

    StringWrapper(StringWrapper&& string_wrapper) {
        std::cout << "移动构造函数" << std::endl;
        // 内部应该移动StringWrapper成员
        str_ = std::move(string_wrapper.str_);
        string_wrapper.str_ = "";
    }

    StringWrapper& operator=(StringWrapper&& string_wrapper) {
        std::cout << "移动赋值运算符" << std::endl;
        str_ = std::move(string_wrapper.str_);
        string_wrapper.str_ = "";
    }

    std::string GetString() {
        return str_;
    }

private:
    std::string str_;
};

int main() {
    std::cout << "初始化 wrap1" << std::endl;
    StringWrapper wrap1("s1");
    

    std::cout << "Before 通过 wrap1 移动构造 wrap2, wrap1 值为" << wrap1.GetString() << std::endl;
    StringWrapper wrap2(std::move(wrap1));
    std::cout << "After wrap1 的资源被转移, wrap1 值为" << wrap1.GetString() << std::endl;
    std::cout << "wrap2 值为" << wrap2.GetString() << std::endl;

    StringWrapper wrap3("s3");
    std::cout << "Before 通过 wrap3 移动赋值给 wrap4, wrap3 值为" << wrap3.GetString() << std::endl;
    StringWrapper wrap4;
    wrap4 = std::move(wrap3);
    std::cout << "After wrap3 的资源被转移, wrap3 值为" << wrap3.GetString() << std::endl;
    std::cout << "wrap4 值为" << wrap4.GetString() << std::endl;

    // 初始化 wrap1
    // 拷贝构造函数
    // Before 通过 wrap1 移动构造 wrap2, wrap1 值为s1
    // 移动构造函数
    // After wrap1 的资源被转移, wrap1 值为
    // wrap2 值为s1
    // 拷贝构造函数
    // Before 通过 wrap3 移动赋值给 wrap4, wrap3 值为s3
    // 移动赋值运算符
    // After wrap3 的资源被转移, wrap3 值为
    // wrap4 值为s3

    return 0;
}