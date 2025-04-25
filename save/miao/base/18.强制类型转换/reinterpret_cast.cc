#include <iostream>
#include <iomanip> // 用于格式化输出

int main() {
    int arr[] = {1, 2, 3};
    for (int i = 0; i < 3; i++) {
        std::cout << *(arr+i) << std::endl;
    }
    char* c = reinterpret_cast<char*>(arr);

    int* int_ptr = reinterpret_cast<int*>(c);
    int* int_ptr2 = reinterpret_cast<int*>(c+4);
    int* int_ptr3 = reinterpret_cast<int*>(c+8);
    int* int_ptr4 = reinterpret_cast<int*>(c+12);

    std::cout << "前4个字节转换回的int值: " << *int_ptr << std::endl; // 1
    std::cout << "中4个字节转换回的int值: " << *int_ptr2 << std::endl; // 2
    std::cout << "后4个字节转换回的int值: " << *int_ptr3 << std::endl; // 3
    std::cout << "后4个字节转换回的int值: " << *int_ptr4 << std::endl; // -1832608512


    return 0;
}