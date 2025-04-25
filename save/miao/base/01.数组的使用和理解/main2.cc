// 针对二维数组的几种遍历方法

#include <iostream>

// 遍历固定大小的二维数组
void func1() {
    int arr[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    // 遍历二维数组
    for (int i = 0; i < 3; i++) { // 遍历行
        for (int j = 0; j < 4; j++) { // 遍历列
            printf("%d ", arr[i][j]);
        }
        printf("\n"); // 每行结束后换行
    }
}

// 遍历作为函数参数的二维数组
void print2DArray(int arr[][4], int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void func2() {
    int arr[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // 调用函数打印二维数组
    print2DArray(arr, 3);
}

//  遍历一维数组模拟的二维数组
void func3() {
    int rows = 3, cols = 4;
    int arr[rows * cols]; // 一维数组模拟二维数组

    // 初始化一维数组
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i * cols + j] = i * cols + j + 1; // 计算索引
        }
    }

    // 遍历一维数组
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", arr[i * cols + j]);
        }
        printf("\n");
    }
}

void func4() {
    int rows = 2, cols = 3;
    int **array = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        array[i] = (int *)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            array[i][j] = i * cols + j + 1;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }

    // 释放内存
    for (int i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}

/**
 * 对于固定大小的二维数组，直接使用嵌套循环遍历。
 * 对于作为函数参数的二维数组，需要指定列数。
 * 对于一维数组模拟的二维数组，通过计算索引访问元素。
 * 对于动态分配的二维数组，使用指针访问元素。
*/
int main() 
{
    func1();

    func2();

    func3();

    func4();
}