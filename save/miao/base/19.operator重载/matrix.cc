#include <iostream>
#include <vector>
#include <exception>

class Matrix {
public:
    Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows, std::vector<double>(cols, 0)) {}
    
    int rows() const { return rows_; }
    int cols() const { return cols_; }

    // 重载()运算符实现访问
    double& operator()(int i, int j) {
        if (i >= rows_ || j >= cols_) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data_[i][j];
    }

    const double& operator()(int i, int j) const {
        if (i >= rows_ || j >= cols_) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data_[i][j];
    }

    Matrix operator*(const Matrix& other) {
        Matrix result(rows_, other.cols_);

        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < other.cols_; j++) {
                double sum = 0;
                for (int k = 0; k < cols_; k++) {
                    // 行 * 列
                    sum += data_[i][k] * other.data_[k][j];
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);


private:
    Matrix() = delete;

    int rows_;
    int cols_;
    std::vector<std::vector<double>> data_;
};

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (int i = 0; i < matrix.rows_; i++) {
        for (int j = 0; j < matrix.cols_; j++) {
            os << matrix.data_[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

int main() {
    Matrix A(2, 3);
    Matrix B(3, 2);

    A(0, 0) = 1; A(0, 1) = 2; A(0, 2) = 3;
    A(1, 0) = 4; A(1, 1) = 5; A(1, 2) = 6;
    std::cout << A << std::endl;

    B(0, 0) = 7; B(0, 1) = 8;
    B(1, 0) = 9; B(1, 1) = 10;
    B(2, 0) = 11; B(2, 1) = 12;

    std::cout << B << std::endl;
    
    Matrix C = A * B;

    std::cout << C << std::endl;

    Matrix D(4, 4);
    
    try {
        D(5, 5);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}