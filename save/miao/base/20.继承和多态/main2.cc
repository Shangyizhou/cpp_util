#include <iostream>
#include <memory>

class Animal {
public:
    Animal() {}
    virtual ~Animal() {}

    virtual void makeSound() = 0;
};

class Dog : public Animal {
public:
    Dog() {}
    ~Dog() { std::cout << "release Dog" << std::endl; }

    void makeSound() override { std::cout << "Dog makeSound" << std::endl; }
};

class Cat : public Animal {
public:
    Cat() {}
    ~Cat() { std::cout << "release Cat" << std::endl; }

    void makeSound() override { std::cout << "Cat makeSound" << std::endl; }
};

int main() {
    std::shared_ptr<Animal> animal_ptr; // 纯虚函数不能有实例

    animal_ptr = std::make_shared<Dog>();
    animal_ptr->makeSound(); // Dog makeSound
    // release Dog

    animal_ptr = std::make_shared<Cat>(); 
    animal_ptr->makeSound(); // Cat makeSound
    // release Cat

    return 0;
}