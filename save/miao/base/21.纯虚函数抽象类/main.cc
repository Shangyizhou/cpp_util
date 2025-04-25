#include <iostream>
#include <memory>

class Animal
{
public:
    Animal() { std::cout << "Base Animal " << std::endl; }
    virtual void makeSound() = 0;
    ~Animal() { std::cout << "Base Animal Relaease" << std::endl; }
};

class Dog : public Animal
{
public:
    Dog() { std::cout << " Dog " << std::endl; }
    void makeSound() override { std::cout << " Dog makeSound" << std::endl; };
    ~Dog() { std::cout << " Dog Relaease" << std::endl; }
};

class Cat : public Animal
{
public:
    Cat() { std::cout << " Cat " << std::endl; }
    void makeSound() override { std::cout << " Cat makeSound" << std::endl; };
    ~Cat() { std::cout << " Cat Relaease" << std::endl; }
};

int main()
{
    // std::shared_ptr<Animal> anima = std::make_shared<Animal>(); 纯虚函数不能被构造

    std::shared_ptr<Animal> anima_ptr = std::make_shared<Dog>();
    anima_ptr->makeSound();
    anima_ptr = std::make_shared<Cat>();
    anima_ptr->makeSound();

    // Base Animal 
    // Cat 
    // Dog Relaease
    // Base Animal Relaease
    // Cat makeSound
    // Cat Relaease
    // Base Animal Relaease
    return 0;
}