// 题目1：
// 1.编写一个enum class，名为Color，包含RED、GREEN、BLUE三个枚举值。
// 2.编写代码尝试直接访问RED（不通过Color::前缀），并理解为什么编译器会报错。
// 3.编写代码将Color::RED显式转换为int类型，并打印结果。

// 题目2：
// 1.编写一个函数，该函数接收一个Color枚举类型的参数，并根据参数值打印出对应的颜色名称。
// 2.验证函数能够正确处理所有Color枚举值。

#include <iostream>

enum class Color {
    RED,
    GREEN,
    BLUE,
};

void func(Color color) {
    switch (color)
    {
    case Color::RED:
        std::cout << "RED" << std::endl;
        break;
    case Color::GREEN:
        std::cout << "GREEN" << std::endl;
        break;
    case Color::BLUE:
        std::cout << "BLUE" << std::endl;
        break;
    default:
        break;
    }
}

int main() {
    // main.cc:36:17: error: ‘RED’ was not declared in this scope
    // RED 是 Color 的成员，必须通过 Color::RED 访问。
    // 传统 enum 允许直接访问 RED，但 enum class 强制作用域限定，避免命名冲突。
    // int color = RED;
    
    // "Color" 类型的值不能用于初始化 "int" 类型的实体C/C++(144)
    // enum class 不会隐式转换为整数，必须用 static_cast 显式转换。
    // int color2 = Color::BLUE; 
    
    int color3 = static_cast<int>(Color::BLUE);
    std::cout << color3 << std::endl;

    func(Color::RED);
    func(Color::BLUE);
    func(Color::GREEN);

}