// 题目：inline函数的基本使用

// 要求：
// 1.定义一个inline函数add，该函数接收两个整数参数并返回它们的和。
// 2.在main函数中调用add函数，并打印结果。

// 验收标准：
// 1.程序能够正确编译并运行。
// 2.add函数被正确地声明为inline。
// 3.main函数中add函数的调用被内联展开（可通过编译器优化设置和检查生成的汇编代码来验证，但这通常不是必须的）。

// 注意：由于inline是一个向编译器的建议，因此编译器可能出于各种原因（如函数体过大、优化级别设置等）选择不内联某些inline函数。因此，在验收时主要关注inline的声明和使用方式是否正确，而不是内联是否实际发生。
#include <iostream>

inline int add(int a, int b) {
    return a + b;
}

int main() {
    std::cout << add(1, 2) << std::endl;
}