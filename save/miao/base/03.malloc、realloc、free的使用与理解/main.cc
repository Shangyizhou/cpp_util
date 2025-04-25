// 题目1：编写一个程序，使用malloc动态分配一个整型数组的空间，允许用户输入数组的大小（不超过100），然后输入数组的元素值，并打印出数组的内 容，最后使用 free 释放内存。

// 题目2：编写一个程序，首先使用malloc分配一个整型数组的空间，并填充一些初始值。然后，使用realloc增加数组的大小，并再次填充新分配的部分。最后，打印出整个数组的内容。最后使用 free 释放内存。

#include <stdio.h>
#include <malloc.h>
#include <alloca.h>

void func1(int size) {
    int *arr = (int*)malloc(sizeof(int) * size);
    

    for (int i = 0; i < size; i++) {
        *(arr + i) = i;
    }

    for (int i = 0; i < size; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");
    free(arr);
    arr = nullptr;
}

void func2(int size, int extern_size) {
    int *arr = (int*)malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        *(arr + i) = i;
    }

    for (int i = 0; i < size; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");

    printf("\n after realloc \n");
    arr = (int*)realloc(arr, sizeof(int) * extern_size);
    for (int i = size; i < extern_size; i++) {
        *(arr + i) = i;
    }

    for (int i = 0; i < extern_size; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");

    free(arr);
}

int main() {
    func1(10);
    func2(10, 20);

    return 0;
}