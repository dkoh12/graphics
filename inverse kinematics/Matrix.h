#pragma once

#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

struct Matrix {
    double A[4][4];

    double& operator()(const int i, const int j);
    double operator()(const int i, const int j) const;
    Matrix operator*(const Matrix& B) const;
    Vector operator*(const Vector& b) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& M);
};

inline double& Matrix::operator()(const int i, const int j) {
    return this->A[i][j];
}

inline double Matrix::operator()(const int i, const int j) const {
    return this->A[i][j];
}

inline std::ostream& operator<<(std::ostream& os, const Matrix& M) {
    os << "(" << M(0, 0) << ", " << M(0, 1) << ", " << M(0, 2) << ", " << M(0, 3) << ", " <<  "\n";
    os << M(1, 0) << ", " << M(1, 1) << ", " << M(1, 2) << ", " << M(1, 3) << ", " << "\n";
    os << M(2, 0) << ", " << M(2, 1) << ", " << M(2, 2) << ", " << M(2, 3) << ", " << "\n";
    os << M(3, 0) << ", " << M(3, 1) << ", " << M(3, 2) << ", " << M(3, 3) << ")";
    return os;
}

inline Matrix Matrix::operator*(const Matrix& B) const {
    Matrix C = {};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                C(i, j) += (*this)(i, k) * B(k, j);
            }
        }
    }
    return C;
}

inline Vector Matrix::operator*(const Vector& b) const {
    Vector bp = b;
    bp.w = 1.;
    Vector c = {};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c[i] += (*this)(i, j) * bp[j];
        }
    }
    return c;
}

Matrix I = {{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
}};

