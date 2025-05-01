#include "Log/logger.h"
#include <vector>
#include <map>


int main() {
    // 基本类型
    int integer = 42;
    float floating = 3.14f;
    double double_val = 2.71828;
    bool boolean = true;
    const char* c_string = "C-style string";
    std::string cpp_string = "C++ string";

    // 复合类型
    std::vector<int> vec = {1, 2, 3};
    std::map<std::string, int> map = {{"one", 1}, {"two", 2}};

    // 打印基本类型
    LOG_INFO("[int] value = {}", integer);
    LOG_INFO("[float] value = {:.2f}", floating);  // 保留2位小数
    LOG_INFO("[double] value = {:.5f}", double_val);
    LOG_INFO("[bool] value = {}", boolean);
    LOG_INFO("[C-string] value = {}", c_string);
    LOG_INFO("[std::string] value = {}", cpp_string);
    LOG_ERROR("[std::string] value = {}", cpp_string);

    for (const auto& item : vec) {
        LOG_INFO("[vector] item = {}", item);
    }

    for (const auto& item : map) {
        LOG_INFO("[map] key = {}, value = {}", item.first, item.second);
    }

    // 指针和地址
    int* ptr = &integer;
    LOG_INFO("[pointer] address = {}", static_cast<void*>(ptr));
    LOG_INFO("[pointer] dereferenced value = {}", *ptr);

    return 0;
}