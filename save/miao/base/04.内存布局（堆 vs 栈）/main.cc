// 题目1：编写一个程序，动态分配一个整型数组的内存空间，用于存储用户输入的5个整数，然后遍历并打印这些整数。最后，释放分配的内存。

// 题目2：编写一个程序，包含两个函数。第一个函数使用栈内存（局部变量）存储并打印一个整数数组；第二个函数使用堆内存动态分配并存储用户输入的整数数组，然后打印并释放内存。通过这两个函数的调用，展示栈内存和堆内存在使用上的区别。


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void heap_memory() {
    int* arr = new int[5];
    for (int i = 0; i < 5; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    delete[] arr;
}

void heap_memory() {
    int *arr = (int*)malloc(5 * sizeof(int));
    printf("please input 5 numbers\n");
    for (int i = 0; i < 5; i++) {
        scanf("%d", arr + i);
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");

    free(arr);    
}

void stack_memory() {
    int arr[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        printf("%d ", *(arr + i));
    }
}

int main() {
    heap_memory();
    stack_memory();

    return 0;
}