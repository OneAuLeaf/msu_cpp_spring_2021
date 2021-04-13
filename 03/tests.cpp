#include <iostream>
#include <string>
#include <cassert>

#include "Matrix.hpp"

// обычное использование матрицы и тестирование вывода в поток
void defaultUse()
{
    // квадратная матрица
    Matrix mat1(3, 3);
    assert(mat1.getRows() == 3 && mat1.getColumns() == 3);

    mat1[1][2] = 5; // индексирование слева
    int32_t x = mat1[1][2]; // индексирование справа
    assert(mat1[1][2] == 5 && x == 5);

    std::cout << "mat1" << mat1 << std::endl;

    // прямоугольная матрица - строк больше чем столбцов
    Matrix mat2(4, 1);
    assert(mat2.getRows() == 4 && mat2.getColumns() == 1);
    mat2[0][0] = 1;
    mat2[1][0] = 2;
    mat2[2][0] = 3;
    mat2[3][0] = 4;

    // прямоугольная матрица - столбцов больше чем строк
    Matrix mat3(1, 4);
    assert(mat3.getRows() == 1 && mat3.getColumns() == 4);
    mat3[0][0] = 1;
    mat3[0][1] = 2;
    mat3[0][2] = 3;
    mat3[0][3] = 4;

    std::cout << "mat2 and mat3" << mat2 << mat3 << std::endl;
}

// проверка работы конструктора копирования и оператора присваивания
void copyAssigmentUse()
{
    Matrix copy(1, 2);
    copy[0][0] = 1;
    copy[0][1] = 2;

    Matrix mat4 = copy; // конструктор копирования
    assert(mat4.getRows() == copy.getRows() && mat4.getColumns() == copy.getColumns());
    assert(mat4[0][0] == copy[0][0] && mat4[0][1] == copy[0][1]);

    mat4[0][0] = 3; // mat4 копия copy и изменения одной не влияют на другую
    copy[0][1] = 4;
    assert(mat4[0][0] != copy[0][0] && mat4[0][1] != copy[0][1]);

    Matrix mat5(1, 1);
    mat5 = mat4; // оператор присваивания
    assert(mat5.getRows() == mat4.getRows() && mat5.getColumns() == mat4.getColumns());
    assert(mat5[0][0] == mat4[0][0] && mat5[0][1] == mat4[0][1]);

    mat5[0][0] = 5; // mat5 копия mat4 и изменения одной не влияют на другую
    mat4[0][1] = 6;
    assert(mat5[0][0] != mat4[0][0] && mat5[0][1] != mat4[0][1]);

    int32_t x = mat5[0][0];
    int32_t y = mat5[0][1];
    size_t rows = mat5.getRows();
    size_t cols = mat5.getColumns();
    mat5 = mat5; // самоприсваивание - ничего не происходит
    assert(mat5.getRows() == rows && mat5.getColumns() == cols);
    assert(mat5[0][0] == x && mat5[0][1] == y);
}

// проверка операций +=, +, *=, ==, !=
void operationsUse()
{
    Matrix mat(2, 2);
    mat[0][0] = 0;
    mat[0][1] = 1;
    mat[1][0] = -1;
    mat[1][1] = -1;

    Matrix mat6(2, 2);
    mat6[0][0] = 1;
    mat6[0][1] = 2;
    mat6[1][0] = 2;
    mat6[1][1] = -1;

    Matrix mat7 = mat6 + mat; // +
    assert(mat7[0][0] == 1 && mat7[0][1] == 3 && mat7[1][0] == 1 && mat7[1][1] == -2);

    mat6 += mat; // +=
    assert(mat6[0][0] == 1 && mat6[0][1] == 3 && mat6[1][0] == 1 && mat6[1][1] == -2);

    assert(mat7 == mat6); // == true
    assert(!(mat7 != mat6)); // != false

    mat6 *= 2; // *=
    assert(mat6[0][0] == 2 && mat6[0][1] == 6 && mat6[1][0] == 2 && mat6[1][1] == -4);

    assert(!(mat7 == mat6)); // == false
    assert(mat7 != mat6); // != true
}

// проверка невозможности операций для матриц разного размера
void differentShapesMatrixs()
{
    Matrix mat(2,2);
    mat[0][0] = 1;
    mat[0][1] = 2;
    mat[1][0] = 2;
    mat[1][1] = -1;

    // не совпадает количество строк
    Matrix mat8(3, 2); 
    try {
        mat8 + mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция +
    } catch (...) {
        assert(false);
    }
    try {
        mat8 += mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция +=
    } catch (...) {
        assert(false);
    }
    try {
        mat8 == mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция ==
    } catch (...) {
        assert(false);
    }
    try {
        mat8 != mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция !=
    } catch (...) {
        assert(false);
    }

    // не совпадает количество столбцов
    Matrix mat9(2, 3); 
    try {
        mat9 + mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция +
    } catch (...) {
        assert(false);
    }
    try {
        mat9 += mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция +=
    } catch (...) {
        assert(false);
    }
    try {
        mat9 == mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция ==
    } catch (...) {
        assert(false);
    }
    try {
        mat9 != mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция !=
    } catch (...) {
        assert(false);
    }

    // не совпадает количество строк и количество столбцов
    Matrix mat10(3, 3); 
    try {
        mat10 + mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция +
    } catch (...) {
        assert(false);
    }
    try {
        mat10 += mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция +=
    } catch (...) {
        assert(false);
    }
    try {
        mat10 == mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция ==
    } catch (...) {
        assert(false);
    }
    try {
        mat10 != mat;
        assert(false);
    } catch (std::invalid_argument& e) {
        // операция !=
    } catch (...) {
        assert(false);
    }
}

// корректность выхода индекса за границы
void indexOutOfOrange()
{
    Matrix mat11(2,2);
    std::string row_error = "number of rows out of range";
    std::string column_error = "number of columns out of range";

    try {
        mat11[3][0];
        assert(false);
    } catch (std::out_of_range& e) {
        // выход за индекс строк
        assert(e.what() == row_error);
    } catch (...) {
        assert(false);
    }
    try {
        mat11[0][3];
        assert(false);
    } catch (std::out_of_range& e) {
        // выход за индекс столбцов
        assert(e.what() == column_error);
    } catch (...) {
        assert(false);
    }
    try {
        mat11[3][3];
        assert(false);
    } catch (std::out_of_range& e) {
        // выход за индекс строк и столбцов, но сначала проверяются строки
        assert(e.what() == row_error);
    } catch (...) {
        assert(false);
    }
}

// использование пустой матрицы
void emptyMatrix()
{
    Matrix mat12; // пустая матрица
    assert(mat12.getRows() == 0 && mat12.getColumns() == 0);
    try {
        mat12[0][0];
        assert(false);
    } catch (std::out_of_range& e) {
        // пустая матрица
    } catch (...) {
        assert(false);
    }
    // но вывод в поток корректен
    std::cout << "empty matrix" << mat12 << std::endl;
}

// проверка матрицы размерностью 1x1
void singleMatrix()
{
    Matrix mat13(1, 1);
    assert(mat13.getRows() == 1 && mat13.getColumns() == 1);

    // индексация
    try {
        mat13[0][0] = 1;
    } catch (std::out_of_range& e) {
        assert(false);
    }
    assert(mat13[0][0] == 1);

    // копирование
    Matrix mat14 = mat13;
    assert(mat14.getRows() == 1 && mat14.getColumns() == 1 && mat14[0][0] == mat13[0][0]);
    
    // операция *=
    mat14 *= 2;
    assert(mat14[0][0] == 2);

    // операция +=
    mat13 += mat14;
    assert(mat13[0][0] == 3);

    // операция +
    Matrix mat15 = mat13 + mat14;
    assert(mat15[0][0] == 5);
    
    // присваивание
    mat15 = mat13;

    // операция ==
    assert(!(mat15 == mat14));
    assert(mat15 == mat13);

    // операция !=
    assert(!(mat15 != mat13));
    assert(mat15 != mat14);

    // вывод в поток
    std::cout << "1x1 matrix" << mat15 << std::endl;
}

int main()
{
    defaultUse();
    copyAssigmentUse();
    operationsUse();
    differentShapesMatrixs();
    indexOutOfOrange();
    emptyMatrix();
    singleMatrix();
}