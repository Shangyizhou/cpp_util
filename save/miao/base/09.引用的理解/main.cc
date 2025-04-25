#include <iostream>
#include <vector>

/**
基本概念理解：
1.理解引用是变量的别名，不是变量本身，也不是变量的拷贝。
2.理解引用在定义时必须被初始化，且一旦被绑定到一个变量上，就不能再被绑定到另一个变量上。

引用的使用：
1.掌握如何通过引用来访问和修改它所引用的变量的值。
2.理解引用在函数参数传递中的作用，特别是与指针相比的优势（如语法简洁性和类型安全性）。

题目：编写一个C++函数，该函数接收一个整型数组、大小和一个整型引用作为参数。函数通过引用参数返回数组中的最大值，并打印出该最大值。
*/

void GetMaxValue(int arr[], int size, int& max_value) {
    if (size <= 0) return; 
    
    max_value = arr[0];    
    for (int i = 0; i < size; i++) {
        if (i > 0 && arr[i] > arr[i - 1]) {
            max_value = arr[i];
        }    
    }
}

int main() {
    int value = 0;
    int a[] = {1, 2, 3, 4, 5};
    GetMaxValue(a, 5, value);
    std::cout << value << std::endl;
}