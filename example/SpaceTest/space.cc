#include "other/space.h"
#include <cassert>
#include <iostream>

using namespace tool; // 使用 logger 命名空间

// 编译期测试函数
constexpr bool test_compile_time() {
    bytes b1(1024);                     // 构造 bytes
    kilobytes kb1(1);                    // 构造 kilobytes
    bytes b2 = kb1;                      // 隐式转换 KB -> Bytes
    assert(b2.count() == 1024);          // 验证转换

    megabytes mb1(2);
    kilobytes kb2 = mb1;                 // 隐式转换 MB -> KB
    assert(kb2.count() == 2048);

    space<double, mega> mb_d(1.5);       // 浮点类型存储
    space<int, kilo> kb_int = mb_d;      // 浮点转整数（截断）
    assert(kb_int.count() == 1536);      // 1.5 MB = 1536 KB

    return true;
}

// 运行时测试函数
void test_runtime() {
    // 基础构造和转换
    terabytes tb(1);
    gigabytes gb = tb;                  // TB -> GB
    std::cout << "1 TB = " << gb.count() << " GB\n";
    assert(gb.count() == 1024);

    // 运算符测试
    kilobytes kb_a(500);
    kilobytes kb_b(1500);
    auto kb_sum = kb_a + kb_b;           // 加法
    assert(kb_sum.count() == 2000);

    // bytes b(512);
    // kilobytes kb_c = kb_a + b;           // 混合单位运算（自动统一单位）
    // assert(kb_c.count() == 501);         // 500 KB + 512 B = 501 KB (512/1024=0.5 KB)

    // 自增/自减
    ++kb_a;
    assert(kb_a.count() == 501);
    kb_a++;
    assert(kb_a.count() == 502);

    // 复合赋值
    megabytes mb1(2);
    mb1 += kilobytes(1024);              // MB += KB
    assert(mb1.count() == 3);           // 2 MB + 1024 KB = 3 MB

    // 负数测试
    space<int, kilo> kb_neg(-1);
    bytes b_neg = kb_neg;
    assert(b_neg.count() == -1024);
}

int main() {
    // 编译期测试
    static_assert(test_compile_time(), "Compile-time test failed");
    constexpr bytes const_b(2048);        // 编译期初始化
    static_assert(const_b.count() == 2048);

    // 运行时测试
    test_runtime();

    // 边界测试
    space<uint64_t, giga> max_gb(UINT64_MAX); // 测试大数值
    std::cout << "Max GB: " << max_gb.count() << "\n";

    std::cout << "All tests passed!\n";
    return 0;
}