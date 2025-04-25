#include <iostream>
#include <memory>

template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}

template<typename T>
class Box {
public:
    Box() {}
    ~Box() {}

    void SetHeight(const T& height) { height_ = height; }
    void SetWidth(const T& width) { width_ = width; }
    void SetLength(const T& length) { length_ = length; }

    T GetHeight() const { return height_; }
    T GetWidth() const { return width_; }
    T GetLength() const { return length_;; }

    T GetVolume() const { return height_ * width_ * length_; }

private:    
    T height_;
    T width_;
    T length_;
};
int main()
{
    Box<int> box1;
    box1.SetHeight(1);
    box1.SetLength(2);
    box1.SetWidth(3);

    std::cout << box1.GetHeight() << std::endl;
    std::cout << box1.GetWidth() << std::endl;
    std::cout << box1.GetLength() << std::endl;
    std::cout << box1.GetVolume() << std::endl; // 6

    Box<double> box2;
    box2.SetHeight(3.0);
    box2.SetLength(2.1);
    box2.SetWidth(3.3);

    std::cout << box2.GetHeight() << std::endl;
    std::cout << box2.GetWidth() << std::endl;
    std::cout << box2.GetLength() << std::endl;
    std::cout << box2.GetVolume() << std::endl; // 20.79


    return 0;
}