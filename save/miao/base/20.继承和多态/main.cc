#include <iostream>
#include <memory>

class Vehicle {
public:
    Vehicle() {}
    virtual ~Vehicle() {}

public:
    std::string color_;
};

class Car : virtual public Vehicle {
public:
    Car() {}
    ~Car() { std::cout << "release Car" << std::endl; }
};

class Boat : virtual public Vehicle {
public:
    Boat() {}
    ~Boat() { std::cout << "release Boat" << std::endl; }
};

class AmphibiousCar : public Car, Boat {
public:
    AmphibiousCar() {}
    ~AmphibiousCar() { std::cout << "release AmphibiousCar" << std::endl; }
};

int main() {
    AmphibiousCar amphibious_car;
    // 没有使用虚继承，存在多份拷贝，不明确color对象，有二义性
    // amphibious_car.color_ = "hello"; "AmphibiousCar::color_" 不明确C/C++(266)
    amphibious_car.color_ = "hello"; // 直接设置唯一继承的color成员，虚继承可以只保留一份
    

    return 0;
}