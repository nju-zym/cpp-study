#include <iostream>
class Matrix {
   private:
    int rows, cols;  // 矩阵的行、列数
    int** data;      // 二维数组管理矩阵

   public:
    // 构造函数，根据给定的行数m和列数c动态分配二维数组内存
    Matrix(int r, int c);
    // 析构函数，释放 data 所分配的内存
    ~Matrix();

    // 友元运算符重载
    friend Matrix operator+(Matrix const& a, Matrix const& b);
    friend Matrix operator-(Matrix const& a, Matrix const& b);
    friend Matrix operator*(Matrix const& a, Matrix const& b);
};