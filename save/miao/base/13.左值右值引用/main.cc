// 题目1：区分左值与右值

// 编写一个C++程序，定义一个函数 printType ，该函数接受一个整型参数，并判断该参数是左值还是右值。由于C++标准库中没有直接判断左值或右值的函数，你可以通过其他方式（如模板特化或std::is_rvalue_reference等）来间接判断，并在控制台输出相应的信息。

// 验收标准：
// 1.程序应能正确识别并输出参数是左值还是右值。（通过type_traits）

// 题目2：利用右值引用实现移动构造函数

// 定义一个简单的类StringWrapper，该类包含一个std::string成员变量。为这个类实现一个移动构造函数，使得当用右值初始化或赋值给StringWrapper对象时，能够利用移动语义来避免不必要的字符串拷贝。

// 验收标准：
// 1.StringWrapper类应包含一个std::string类型的成员变量。
// 2.正确实现移动构造函数，确保当用右值初始化或赋值给StringWrapper对象时，能够利用移动语义。
// 3.可以通过编写测试代码来验证移动构造函数是否按预期工作，例如，通过比较移动前后的对象状态以及检查是否有额外的字符串拷贝发生

// 题目3：实现完美转发

// 编写一个模板函数forwarder，该函数接受任意类型和数量的参数，并使用完美转发将这些参数转发给另一个函数（可以是同一个类或命名空间中的另一个函数）。为了简化问题，可以假设要转发的目标函数是已知的，并且其签名也已给出。

// 验收标准：
// 1.forwarder函数应使用模板和std::forward来实现完美转发。
// 2.应能正确处理左值和右值作为参数的情况，确保它们被按原始类型转发给目标函数。
#include <iostream>
#include <string>
#include <type_traits>

// 目标函数示例
void target_function(int& x) {
    std::cout << "Lvalue reference: " << x << std::endl;
}

void target_function(int&& x) {
    std::cout << "Rvalue reference: " << x << std::endl;
}

// 可变参数版本的模板函数
template <typename... Args>
void forwarder(Args&&... args) {
    // 完美转发所有参数到目标函数
    target_function(std::forward<Args>(args)...);
}


class StringWrapper {
public:
    StringWrapper() {}
    StringWrapper(const std::string& str) : str_(str) {}
    StringWrapper(StringWrapper&& str) {
        std::cout << "移动语义" << std::endl;
        str_ = str.str_;
        str.str_ = "";
    }

    std::string GetString() const { return str_; }

private:
    std::string str_;
};

template<typename T>
void printType(T&& value) {
    if (std::is_lvalue_reference<decltype(value)>::value) {
        std::cout << "left value" << std::endl;
    } else if (std::is_rvalue_reference<decltype(value)>::value) {
        std::cout << "right value" << std::endl;
    } else {
        std::cout << "纯右值" << std::endl;
    }
}

int main() {
    int x = 42;
    printType(42);
    printType(std::move(42));
    printType(x);

    StringWrapper s1("hello");
    StringWrapper s2(std::move(s1));
    std::cout << "s1 " << s1.GetString() << std::endl;

    std::string s3 = "hello";
    std::string s4 = std::move(s3);
    std::cout << "after move s3 " << s3 << std::endl;

    // 测试左值
    int a = 42;
    forwarder(a);       
    
    // 测试右值
    forwarder(100);     
    
    return 0;
}