#include <stdexcept>

#include "Matrix.hpp"

Matrix::ProxyRow::ProxyRow(int32_t *data, size_t size) : size_{size}, data_{data} {}
        
int32_t Matrix::ProxyRow::operator[](size_t j) const
{
    if (j >= size_) {
        throw std::out_of_range("number of columns out of range");
    }
    return data_[j];
}

int32_t& Matrix::ProxyRow::operator[](size_t j)
{
    if (j >= size_) {
        throw std::out_of_range("number of columns out of range");
    }
    return data_[j];
}

Matrix::Matrix() : rows_{0}, columns_{0}, data_{nullptr} {}

Matrix::Matrix(size_t rows, size_t columns)
{
    if (!rows || !columns) {
        rows_ = 0;
        columns_ = 0;
        data_ = nullptr;
    } else {
        rows_ = rows;
        columns_ = columns;
        data_ = new int32_t[rows_ * columns_];
        for (size_t i = 0; i < rows_ * columns_; ++i) {
            data_[i] = 0;
        }
    }
}

Matrix::Matrix(const Matrix& other) : rows_{other.rows_}, columns_{other.columns_}
{
    data_ = new int32_t[rows_ * columns_];
    for (size_t i = 0; i < rows_ * columns_; ++i) {
        data_[i] = other.data_[i];
    }
}

Matrix& Matrix::operator= (const Matrix& other)
{
    if (this != &other) {
        delete [] data_;
        rows_ = other.rows_;
        columns_ = other.columns_;
        data_ = new int32_t[rows_ * columns_];
        for (size_t i = 0; i < rows_ * columns_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    return *this;
}

Matrix::Matrix(Matrix&& other) : rows_{other.rows_}, columns_{other.columns_}, data_{other.data_}
{
    other.rows_ = 0;
    other.columns_ = 0;
    other.data_ = nullptr;
}

Matrix& Matrix::operator= (Matrix&& other)
{
    if (this != &other) {
        delete [] data_;
        rows_ = other.rows_;
        columns_ = other.columns_;
        data_ = other.data_;
        other.rows_ = 0;
        other.columns_ = 0;
        other.data_ = nullptr;
    }
    return *this;
}

Matrix::~Matrix()
{
    delete [] data_;
}
    
const Matrix::ProxyRow Matrix::operator[](size_t i) const
{
    if (i >= rows_) {
        throw std::out_of_range("number of rows out of range");
    }
    return Matrix::ProxyRow(data_ + i * columns_, columns_);
}

Matrix::ProxyRow Matrix::operator[](size_t i)
{
    if (i >= rows_) {
        throw std::out_of_range("number of rows out of range");
    }
    return Matrix::ProxyRow(data_ + i * columns_, columns_);
}

Matrix& Matrix::operator*=(int32_t value)
{
    for (size_t i = 0; i < rows_ * columns_; ++i) {
        data_[i] *= value;
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    if (rows_ != other.rows_ || columns_ != other.columns_) {
        throw std::invalid_argument("cannot sum operands with different shapes");
    }
    for (size_t i = 0; i < rows_ * columns_; ++i) {
        data_[i] += other.data_[i];
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Matrix& mat)
{
    out << std::endl << "[";
    for (size_t i = 0; i < mat.rows_; ++i) {
        out << "[";
        for (size_t j = 0; j < mat.columns_; ++j) {
            if (j < mat.columns_ - 1) {
                out << mat.data_[i * mat.columns_ + j] << ", ";
            } else {
                out << mat.data_[i * mat.columns_ + j] << "]";
            }
        }
        if (i < mat.rows_ - 1) {
            out << std::endl;
        }
    }
    out << "]";
    
    return out;
}

bool operator==(const Matrix& mat1, const Matrix& mat2)
{
    if (mat1.rows_ != mat2.rows_ || mat1.columns_ != mat2.columns_) {
        throw std::invalid_argument("cannot compare operands with different shapes");
    }
    for (size_t i = 0; i < mat1.rows_ * mat2.columns_; ++i) {
        if (mat1.data_[i] != mat2.data_[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const Matrix& mat1, const Matrix& mat2)
{
    return !(mat1 == mat2);
}

Matrix operator+ (Matrix mat1, const Matrix& mat2)
{
    mat1 += mat2;
    return mat1;
}