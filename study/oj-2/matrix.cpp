#include "matrix.hpp"

// 构造函数
Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    // TODO: 根据给定的行数m和列数c动态分配二维数组内存
    data = new int*[rows];

    for (int i = 0; i < rows; i++) {
        data[i] = new int[cols];
    }

}

// 析构函数
Matrix::~Matrix() {
    // TODO: 释放 data 所分配的内存
    for (int i = 0; i < rows; i++) {
        delete[] data[i];
    }
    delete[] data;
}

// 矩阵加法
Matrix operator+(Matrix const& a, Matrix const& b) {
    // TODO: 加法运算符重载
    Matrix res(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            res.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return res;
}

// 矩阵减法
Matrix operator-(Matrix const& a, Matrix const& b) {
    // TODO: 减法运算符重载
    Matrix res(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            res.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    return res;
}

// 矩阵乘法
Matrix operator*(Matrix const& a, Matrix const& b) {
    // TODO: 乘法运算符重载
    Matrix res(a.rows, b.cols);
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < b.cols; j++) {
            res.data[i][j] = 0;
            for (int k = 0; k < a.cols; k++) {
                res.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return res;
}
