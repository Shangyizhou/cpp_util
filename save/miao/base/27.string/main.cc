#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

void func1() {
    std::string str = "hello world";

    str.append(" hello");
    // hello world hello
    std::cout << str << std::endl;

    str.insert(3, "C++");
    // helC++lo world hello
    std::cout << str << std::endl;

    int pos = str.find("world");
    if (pos != std::string::npos) {
        str.erase(pos, 5);
        // world pos 9
        std::cout << "world pos " << pos << std::endl;
        // helC++lo  hello
        std::cout << str << std::endl;
    }

    pos = str.find("hel");
    if (pos != std::string::npos) {
        str.replace(pos, strlen("hello"), "test_replace");
        // test_replace+lo  hello
        std::cout << str << std::endl;
    }

}

void func2() {
    const char* cstr = "C-style string!";

    std::string str(cstr);
    std::cout << str << std::endl;

    str.append(" hello");
    std::cout << str.c_str() << std::endl;

    // str 销毁，此处指向释放后的内存
    const char* c_str = str.c_str();

    // fix
    char* safe_copy = new char[str.length() + 1];
    strcpy(safe_copy, str.c_str());
    std::cout << safe_copy << std::endl;

}

int main() {
    func2();
    
    return 0;
}