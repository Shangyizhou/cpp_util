#include <stdio.h>
#include <stdlib.h>

// 定义宏：检查指针是否为 NULL，如果是则执行指定操作并返回
#define CHECK_NULL_AND_DO(ptr, action, ret_val) \
    do { \
        if ((ptr) == NULL) { \
            action; \
            return (ret_val); \
        } \
    } while (0)

typedef struct {
    char* data;
    size_t length;
    size_t capacity;
} DynamicString;

DynamicString* CreateDynamicString(size_t cap) {
    DynamicString* str = (DynamicString*)malloc(sizeof(DynamicString));
    str->data = (char*)malloc(cap * sizeof(char));
    str->length = 0;
    str->capacity = cap;
}

void AppendChar(DynamicString* str, char c) {
    if (str->length >= str->capacity) {
        str->capacity *= 2;
        str->data = (char*)realloc(str->data, str->capacity * sizeof(char));
        if (str->data == NULL) {
            printf("内存扩展失败");
            exit(1);
        }
    }
    str->data[str->length++] = c;
}

void FreeDynamicString(DynamicString* str) {
    if (str) {
        if (str->data) {
            free(str->data);
        }
        free(str);
    }
}

int main() {
    DynamicString* str = CreateDynamicString(2);
    if (str == NULL) {
        printf("内存分配失败");
        exit(1);
    }

    // 添加字符
    AppendChar(str, 'H');
    AppendChar(str, 'e');
    AppendChar(str, 'l');
    AppendChar(str, 'l');
    AppendChar(str, 'o');

    // 打印字符串
    if (str && str->data) {
        printf("%s\n", str->data);
    }

    // 释放内存
    FreeDynamicString(str);
    return 0;
}