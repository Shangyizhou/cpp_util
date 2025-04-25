#include <iostream>

class Complex {
public:
    Complex() {}
    
    explicit Complex(int real, int virturl) : real_(real), virtual_(virturl) {}
    
    Complex operator+(const Complex& complex) {
        Complex c1;
        c1.real_ = real_ + complex.real_;
        c1.virtual_ = virtual_ + complex.virtual_;
        return c1;
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& complex);

private:
    int real_;
    int virtual_;

};

std::ostream& operator<<(std::ostream& os, const Complex& complex) {
    os << "(" << complex.real_ << "," << complex.virtual_ << ")"; 
}

int main() {
    Complex c1(1, 2);
    Complex c2(3, 2);

    std::cout << c1 << std::endl; // (1,2)
    std::cout << c2 << std::endl; // (3,2)
    std::cout << c1 + c2 << std::endl; // (4,4)

    return 0;
}