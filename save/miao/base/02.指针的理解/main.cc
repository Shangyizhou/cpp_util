#include <iostream>

void func1() {
    int arr[] = {1, 2, 3, 4, 5};
    int *p = arr;
    for (int i = 0; i < 5; i++) {
        std::cout << *p << " ";
        p++;
    }
    std::cout << std::endl;
}

void func2(int arr[], int size) {
    int *p = arr;
    for (int i = 0; i < 5; i++) {
        *p = *p * 2;
        p++;
    }

    std::cout << "-------------------------" << std::endl;
    
    for (int i = 0; i < 5; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    func1();

    int arr[] = {1, 2, 3, 4, 5};
    func2(arr, 5);
}