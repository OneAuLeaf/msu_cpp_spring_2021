#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstdlib>
#include <ostream>

class Matrix
{
    class ProxyRow
    {
    public:
        ProxyRow() = default;
        ProxyRow(int32_t *data_, size_t size);
        
        int32_t operator[](size_t j) const;
        int32_t& operator[](size_t j);

        ~ProxyRow() = default;
    private:
        size_t size_;
        int32_t *data_;
    };
public:
    Matrix();
    Matrix(size_t rows, size_t columns);
    Matrix(const Matrix& other);
    Matrix& operator= (const Matrix& other);
    Matrix(Matrix&& other);
    Matrix& operator= (Matrix&& other);

    size_t getRows() const { return rows_; }
    size_t getColumns() const { return columns_; }
    
    const ProxyRow operator[](size_t i) const;
    ProxyRow operator[](size_t i);
    
    Matrix& operator*=(int32_t value);
    Matrix& operator+=(const Matrix& other);

    ~Matrix();

    friend std::ostream& operator<<(std::ostream& out, const Matrix& mat);
    friend bool operator==(const Matrix& mat1, const Matrix& mat2);
    friend bool operator!=(const Matrix& mat1, const Matrix& mat2);
    friend Matrix operator+ (Matrix mat1, const Matrix& mat2);
private:
    size_t rows_;
    size_t columns_;
    int32_t *data_;
};

#endif