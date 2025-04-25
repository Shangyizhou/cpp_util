#include <iostream>
#include "other/any.h"  // 包含上面的Any类定义

struct Person {
    std::string name;
    int age;
};

int main() {
    // 1. 基本类型使用
    tool::Any a = 10;
    std::cout << "int value: " << a.AnyCast<int>() << std::endl;

    // 2. 字符串
    tool::Any b = std::string("hello");
    std::cout << "string value: " << b.AnyCast<std::string>() << std::endl;

    // 3. 自定义类型
    Person p{"Alice", 30};
    tool::Any c = p;
    Person p2 = c.AnyCast<Person>();
    std::cout << "Person: " << p2.name << ", " << p2.age << std::endl;

    // 4. 类型检查
    if (c.Is<Person>()) {
        std::cout << "c contains a Person" << std::endl;
    }

    // 5. 空值检查
    tool::Any empty;
    if (empty.IsNull()) {
        std::cout << "empty is null" << std::endl;
    }

    // 6. 错误类型转换会抛出异常
    // try {
    //     double d = a.AnyCast<double>();  // 错误：a存储的是int
    // } catch (...) {
    //     std::cout << "Bad any cast!" << std::endl;
    // }

    return 0;
}