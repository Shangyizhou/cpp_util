// 题目1：基本异常处理

// 编写一个C++程序，其中包含一个函数divide，该函数接收两个整数作为参数，并返回它们的除法结果。如果除数为0，则该函数应抛出一个std::runtime_error异常。在main函数中，调用divide函数并捕获可能抛出的异常，打印出异常信息。

// 验收标准：
// 1.程序应能正确执行除法运算，并在除数为0时抛出std::runtime_error异常。
// 2.main函数应能捕获并打印出异常信息。

// 题目2：自定义异常处理

// 定义一个自定义异常类 DivideByZeroException ，该类继承自std::exception，并实现what()方法以返回“Cannot divide by zero”作为异常描述。然后，修改题目1中的divide函数，在除数为0时抛出DivideByZeroException异常。最后，在main函数中捕获并处理这个自定义异常。

// 验收标准：
// 1.DivideByZeroException类应正确实现，并能在除数为0时被抛出。
// 2.main函数应能捕获并打印出DivideByZeroException的自定义异常信息。

// 题目3：多层异常处理

// 编写一个程序，该程序包含三个函数：A、B和C。函数A调用函数B，函数B调用函数C。函数C负责执行某个可能抛出异常的操作（如题目1或题目2中的除法运算）。如果发生异常，异常应能被A函数中的try-catch块捕获并处理。

// 验收标准：
// 1.程序应能正确地在多层函数调用中传播异常。
// 2.异常应能被最外层的try-catch块（在A函数中）捕获并处理。
// 3.应确保在捕获异常后，程序能够继续执行其他任务。

#include <iostream>
#include <exception>
#include <stdexcept> 

double divide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("除数为0");
    }
    return static_cast<double>(a) / b;
}

class DivideByZeroException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Cannot divide by zero";
    }
};

double divide2(int a, int b) {
    if (b == 0) {
        throw DivideByZeroException();
    }
    return static_cast<double>(a) / b;
}

double C(int a, int b) {
    if (b == 0) {
        throw DivideByZeroException();
    }
    return static_cast<double>(a) / b;
}

double B(int a, int b) {
    return C(a, b);
}

double A(int a, int b) {
    try {
        return B(a, b);
    } catch (const DivideByZeroException& e) {
        std::cerr << "错误发生: " << e.what() << std::endl;
    }
}

int main() {
    int a, b;
    
    std::cout << "请输入两个整数（被除数和除数）：";
    std::cin >> a >> b;
    
    try {
        double result = A(a, b);
        std::cout << "除法结果: " << result << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "错误发生: " << e.what() << std::endl;
    } catch (const DivideByZeroException& e) {
        std::cerr << "错误发生: " << e.what() << std::endl;
    }

    std::cout << "继续执行" << std::endl;

    return 0;
}