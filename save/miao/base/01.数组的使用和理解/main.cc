/**
题目1：数组声明与初始化
编写一个程序，声明一个整型数组，并初始化为{1, 3, 5, 7, 9}，然后遍历并打印数组中的每个元素。

题目2：数组元素查找
编写一个程序，声明一个整型数组，并初始化为一些随机整数。编写一个函数，该函数接受数组、数组大小和要查找的元素作为参数，如果找到该元素，则返回其在数组中的索引；如果未找到，则返回-1。在main函数中测试该函数。

题目3：数组排序
编写一个程序，声明一个整型数组，并初始化为一些未排序的整数。使用冒泡排序算法对数组进行排序，并打印排序后的数组。

题目4：二维数组操作
编写一个程序，声明并初始化一个二维整型数组（如矩阵），然后编写一个函数，该函数计算并返回矩阵中所有元素的和。在main函数中测试该函数。

题目5：数组与字符串（额外）
虽然字符串在C++中通常通过std::string处理，但了解C风格字符串（字符数组）的处理也是有益的。编写一个程序，声明一个字符数组作为字符串，并使用循环结构遍历字符串，打印出每个字符及其ASCII码值。
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>

int* generateRandomArray(int size) {
    // 动态分配内存
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }

    // 设置随机数种子
    srand(time(NULL));

    // 填充随机数
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100; // 生成 0 到 99 的随机数
    }

    return arr;
}

// 分割线函数
void printSeparator(const char *functionName) {
    printf("\n----------------------------------------\n");
    printf("执行函数: %s\n", functionName);
    printf("----------------------------------------\n\n");
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void func1() {
    int arr[] = {1, 3, 5, 7, 9};
    int size = sizeof(arr) / sizeof(arr[0]);
    printArray(arr, size);
}

int func2(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}


void bubble_sort(int arr[], int n);
void func3(int arr[], int size) {
    bubble_sort(arr, size);
    printArray(arr, size);
}

void func4(int arr[][2], int row) {
    int sum = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < 2; j++) {
            int num = arr[row][j];
            sum += num;
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "num is " << sum << std::endl;
}

void func5(char str[]) {
    int size = sizeof(str) / sizeof(char);
    for (int i = 0; i < size; i++) {
        std::cout << str[i] << " " << static_cast<int>(str[i]) << std::endl; 
    }
}


void bubble_sort(int arr[], int n) {
    bool swap_flag = false;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swap_flag = true;
            }
        }
        if (!swap_flag) {
            std::cout << "本轮没有交换，说明已经排序成功，提早退出" << std::endl;
            break;
        }
    }
}

void printArraySize(int arr[]) {
    // 在函数内部，arr 已经退化为指针类型
    printf("Size of arr in function: %lu bytes\n", sizeof(arr));
}

int main()
{
    printSeparator("func1");
    func1();

    printSeparator("func2");
    int arr[] = {1, 3, -1, 5, 2, 7, 9};
    int size = sizeof(arr) / sizeof(arr[0]);
    int index = func2(arr, size, 5);
    std::cout << "index is " << index;

    printSeparator("func3");
    func3(arr, size);

    int arr2[2][2] = {
        {1, 2},
        {3, 4}
    };

    printSeparator("func4");
    func4(arr2, 2);

    printSeparator("func5");
    char str[] = {'a', 'b', 'c'};
    func5(str);

    int myArray[10] = {0};

    // 在 main 函数中，myArray 是一个数组
    printf("Size of myArray in main: %lu bytes\n", sizeof(myArray));

    // 将数组传递给函数
    printArraySize(myArray);

    return 0;
}