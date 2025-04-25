#include <iostream>
#include <vector>
#include <memory>

class Shape {
public:
    Shape() {}
    virtual ~Shape() { std::cout << "release Shape" << std::endl; }

};

struct Point {
    Point() : x_(0), y_(0) {}
    double x_;
    double y_;
    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x_ << "," << point.y_ << ")";
    return os;
}

class Circle : virtual public Shape {
public:
    Circle() {}
    ~Circle() { std::cout << "release Circle" << std::endl; }

private:
    double radius_;
    Point point_;
};

class Rectangle : virtual public Shape {
public:
    Rectangle() {}
    ~Rectangle() { std::cout << "release Rectangle" << std::endl; }

private:
    double width_;
    double height_; 
    Point top_left_;       
};


int main() {
    std::vector<std::shared_ptr<Shape>> shape_list;
    std::shared_ptr<Shape> shape_circle = std::make_shared<Circle>();
    std::shared_ptr<Shape> shape_rectangle = std::make_shared<Rectangle>();
    shape_list.push_back(shape_circle);
    shape_list.push_back(shape_rectangle);

    // 符合，析构函数先释放子类再释放父类
    // 如果父类没有虚析构函数，那么子类的析构函数不会调用，会造成内存泄漏
    // release Circle
    // release Shape
    // release Rectangle
    // release Shape

    return 0;
}