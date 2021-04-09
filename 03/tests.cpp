#include <iostream>
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

    Matrix mat5 = copy; // конструктор копирования
    assert(mat5.getRows() == copy.getRows() && mat5.getColumns() == copy.getColumns());
    assert(mat5[0][0] == copy[0][0] && mat5[0][1] == copy[0][1]);

    mat5[0][0] = 3; // mat5 копия copy и изменения одной не влияют на другую
    copy[0][1] = 4;
    assert(mat5[0][0] != copy[0][0] && mat5[0][1] != copy[0][1]);

    Matrix mat6(1, 1);
    mat6 = mat5; // оператор присваивания
    assert(mat6.getRows() == mat5.getRows() && mat6.getColumns() == mat5.getColumns());
    assert(mat6[0][0] == mat5[0][0] && mat6[0][1] == mat5[0][1]);

    mat6[0][0] = 5; // mat6 копия mat5 и изменения одной не влияют на другую
    mat5[0][1] = 6;
    assert(mat6[0][0] != mat5[0][0] && mat6[0][1] != mat5[0][1]);

    int32_t x = mat6[0][0];
    int32_t y = mat6[0][1];
    size_t rows = mat6.getRows();
    size_t cols = mat6.getColumns();
    mat6 = mat6; // самоприсваивание - ничего не происходит
    assert(mat6.getRows() == rows && mat6.getColumns() == cols);
    assert(mat6[0][0] == x && mat6[0][1] == y);
}

// проверка операций +=, +, *=, ==, !=
void operationsUse()
{
    Matrix mat(2, 2);
    mat[0][0] = 0;
    mat[0][1] = 1;
    mat[1][0] = -1;
    mat[1][1] = -1;

    Matrix mat7(2, 2);
    mat7[0][0] = 1;
    mat7[0][1] = 2;
    mat7[1][0] = 2;
    mat7[1][1] = -1;

    Matrix mat8 = mat7 + mat; // +
    assert(mat8[0][0] == 1 && mat8[0][1] == 3 && mat8[1][0] == 1 && mat8[1][1] == -2);

    mat7 += mat; // +=
    assert(mat7[0][0] == 1 && mat7[0][1] == 3 && mat7[1][0] == 1 && mat7[1][1] == -2);

    assert(mat8 == mat7); // ==

    mat7 *= 2; // *=
    assert(mat7[0][0] == 2 && mat7[0][1] == 6 && mat7[1][0] == 2 && mat7[1][1] == -4);

    assert(mat8 != mat7); // !=

    Matrix mat9(3,2);

    try
    {
        mat9 + mat7;
        mat9 += mat7;
        mat9 == mat7;
        mat9 != mat7;
    }
    catch (std::invalid_argument& e)
    {
        // запрещены операции с разными размерами
    }
    catch (...)
    {
        assert(false);
    }
}

// использование пустой матрицы
void emptyMatrix()
{
    Matrix mat9; // пустая матрица
    assert(mat9.getRows() == 0 && mat9.getColumns() == 0);
    try
    {
        mat9[0][0];
    }
    catch (std::out_of_range& e)
    {
        // пустая матрица
    }
    catch (...)
    {
        assert(false);
    }
}

// корректность выхода индекса за границы
void indexOutOfOrange()
{
    Matrix mat10(1,2);
    try
    {
        mat10[3][0];
        mat10[0][3];
        mat10[3][3];
    }
    catch (std::out_of_range& e)
    {
        // выход за границы
    }   
    catch (...)
    {
        assert(false);
    }
        
}

int main()
{
    defaultUse();
    copyAssigmentUse();
    operationsUse();
    emptyMatrix();
    indexOutOfOrange();
}