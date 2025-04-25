#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} DynamicArray;

DynamicArray* createDynamicArray(size_t capacity) {
    DynamicArray* array = (DynamicArray*)malloc(sizeof(DynamicArray));
    array->data = (int*)malloc(capacity * sizeof(int));
    array->size = 0;
    array->capacity = capacity;
    return array;
}

void pushBack(DynamicArray* arr, int value) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = (int*)realloc(arr->data, arr->capacity * sizeof(int));
    }
    arr->data[arr->size++] = value;
}

void freeDynamicArray(DynamicArray* arr) {
    free(arr->data);
    free(arr);
}

int main() {
    DynamicArray *arr = createDynamicArray(2);
    if (arr == NULL) {
        printf("内存分配失败\n");
        return 1;
    }

    // 添加元素
    pushBack(arr, 10);
    printf("size: %d capacity: %d\n", arr->size, arr->capacity);
    pushBack(arr, 20);
    printf("size: %d capacity: %d\n", arr->size, arr->capacity);
    pushBack(arr, 30); // 超出初始容量，自动扩展
    printf("size: %d capacity: %d\n", arr->size, arr->capacity);

    // 打印数组
    for (size_t i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }

    // 释放内存
    freeDynamicArray(arr);
    return 0;
}