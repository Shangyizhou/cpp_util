// 题目1：编写一个C语言程序，定义一个函数int add(int a, int b)，该函数返回两个整数的和。然后定义一个函数指针变量pAdd，将其指向add函数，并通过pAdd调用add函数，计算并打印3和5的和。

// 题目2：编写一个C语言程序，定义一个指针函数int* findMax(int arr[], int size)，该函数遍历一个整数数组，返回指向数组中最大值的指针。在主函数中定义一个整数数组，调用findMax函数，并通过返回的指针打印数组中的最大值。

#include <stdio.h>
#include <limits.h> // 包含 INT_MIN 的定义

int add(int a, int b) {
    int c = a + b;
    printf("%d\n", a + b);
    return c;
}

int* findMax(int arr[], int size) {
    int max = INT_MIN;
    int *index = 0;
    int *p = arr;
    for (int i = 0; i < size; i++) {
        int n = *(arr + i);
        if (n > max) {
            max = n;
            index = arr + i;
        }
    }
    return index;
}

typedef int (*AddFunc)(int, int);

int main() {
    AddFunc func_ptr;
    func_ptr = add;
    func_ptr(1, 2);

    int arr[] = {-1, 0, 1, 2, 3};
    int* max = findMax(arr, sizeof(arr) / sizeof(int));
    printf("get max number %d", *(max));

    return 0;
}