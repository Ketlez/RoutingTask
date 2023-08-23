#pragma once
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

class Matrix {
public:
    Matrix(int size = 0, double values = 0);

    // доступ к элементам по индексам
    double& item(int row, int column);
    double& operator()(int row, int column);
    // получение "реальных" идексов
    // e.g. если была удалена строка 1 rowIndex(1) == 2

    int rowIndex(int row) const;
    int columnIndex(int column) const;

    // размерность матрицы
    int size() const;
    // вывод на экран
    void print() const;
    void printEndl() const;

    // удаление строки и столбца по индексам
    void removeRowColumn(int row, int column);

    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;

private:
    // инициализация "реальных" номеров строк и столбцов
    void initRowsColumns();

    std::vector<std::vector<double>> _items;
    // "реальные" номера строк
    std::vector<int> _rows;
    // "реальные" номера столбцов
    std::vector<int> _columns;
};


