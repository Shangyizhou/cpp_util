#include <iostream>
#include <array>

void func1() {
    int arr[] = {1, 2, 3, 4, 5}; 
    // 1 2 3 4 5
    for (const auto& i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void func2() {
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    // 1 2 3 4 5
    for (const auto& i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void func3(std::array<int, 5>& arr) {
    for (auto& i : arr) {
        i *= 2;
    }
}

int main() {
    func1();
    func2();

    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    func3(arr);
    // 2 4 6 8 10 
    for (const auto& i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}