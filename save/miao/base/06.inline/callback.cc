#include <iostream>
#include <functional>
#include <vector>
#include <string>

void CalculateFunc(std::vector<int>& vec, std::function<int(int)> callback) {
    for (auto& i : vec) {
        i = callback(i);
    }
}

int func(int x) {
    return x * x;
}

int main() {
    std::vector<int> a{1, 2, 3};
    CalculateFunc(a, func);
    for (auto& i : a) {
        std::cout << i << " ";
    }
    // 1 4 9
    std::cout << std::endl;

    return 0;
}