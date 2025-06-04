#include <iostream>
#include <ratio>

void func1() {
    typedef std::ratio<2, 4> two_fourths;  // 自动约分为1/2
    std::cout << "two_fourths= " << two_fourths::num << "/" << two_fourths::den << std::endl;

    typedef std::ratio<4, -6> negative_ratio;  // 负号移至分子
    std::cout << "negative_ratio= " << negative_ratio::num << "/" << negative_ratio::den << std::endl;
}

void func2() {
    typedef std::ratio<1, 3> one_third;
    typedef std::ratio<1, 2> one_half;

    // 加法
    typedef std::ratio_add<one_third, one_half> sum;
    std::cout << "sum= " << sum::num << "/" << sum::den
              << " (" << (double(sum::num) / sum::den) << ")" << std::endl;

    // 乘法
    typedef std::ratio_multiply<one_third, one_half> product;
    std::cout << "product= " << product::num << "/" << product::den << std::endl;
}

int main() {
    func1();

    func2();

    return 0;
}