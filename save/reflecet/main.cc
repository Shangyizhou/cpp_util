#include "ClassFactory.h"
#include <iostream>

class Foo
{
public:
    Foo() = default;
    ~Foo() = default;

    void showInfo()
    {
        std::cout << "class Name Foo, function = showInfo" << std::endl;
    }
public:
    std::string name_;
};

int main()
{
    ClassFactory* factory = ClassFactory::getInstance();
    Foo* f = (Foo*)factory->createClass("Foo");

    return 0;
}

