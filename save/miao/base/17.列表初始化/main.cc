#include <iostream>
#include <string>
#include <vector>

class Box {
public:
    Box() = default;
    Box(std::initializer_list<int> list) {
        std::cout << "initializer_list构造" << std::endl;
        if (list.size() != 2) {
            throw std::invalid_argument("Box requires exactly 2 dimensions");
        }
        auto it = list.begin();
        width_ = *it;
        height_ = *(it + 1);
    }
    Box(const int& width, const int& height) : width_(width), height_(height) {
        std::cout << "有参构造" << std::endl;
    }
    void ShowBox() {
        std::cout << "(" << width_ << ", " << height_ << ")" << std::endl;
    }
private:
    int width_ = 0;
    int height_ = 0;
};

int main() {
    Box b1;                  
    Box b2(10, 20);  // 有参构造  
    Box b3{30, 40};  // initializer_list构造
    
    b1.ShowBox();
    b2.ShowBox();
    b3.ShowBox();

    try {
        Box b4{1,2,3};      
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    return 0;

}