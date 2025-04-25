#include <iostream>
#include <vector>
#include <memory>

class Base {
public:
    Base() {}

    virtual void GetInfo() { std::cout << "Base func " << std::endl; }
};

class Derived : public Base {
public:
    Derived() {}

    void GetInfo() override { std::cout << "Derived func" << std::endl; }
};

class Derived2 : public Base {
public:
    Derived2() {}

    void GetInfo() override { std::cout << "Derived2 func" << std::endl; }
};

class Derived3 : public Base {
public:
    Derived3() {}

    void GetInfo() override { std::cout << "Derived3 func" << std::endl; }
};

void ShowInfo(std::shared_ptr<Base> base_ptr) {
    if (std::dynamic_pointer_cast<Derived2>(base_ptr)) {
        std::cout << "Derived 2 Type";
        base_ptr->GetInfo();
    } else if (std::dynamic_pointer_cast<Derived3>(base_ptr)) {
        std::cout << "Derived 3 Type";
        base_ptr->GetInfo();
    }
}

int main() {
    auto base_ptr = std::make_shared<Base>();
    base_ptr->GetInfo(); // Base func 

    auto deriver_ptr = std::make_shared<Derived>();
    deriver_ptr->GetInfo(); // Derived func

    base_ptr = deriver_ptr;
    base_ptr->GetInfo(); // Derived func

    auto deriver2_ptr = std::make_shared<Derived2>();
    auto deriver3_ptr = std::make_shared<Derived3>();
    ShowInfo(deriver2_ptr);
    ShowInfo(deriver3_ptr);

    return 0;
}