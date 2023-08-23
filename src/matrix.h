#pragma once
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

class Matrix {
public:
    Matrix(int size = 0, double values = 0);

    // ������ � ��������� �� ��������
    double& item(int row, int column);
    double& operator()(int row, int column);
    // ��������� "��������" �������
    // e.g. ���� ���� ������� ������ 1 rowIndex(1) == 2

    int rowIndex(int row) const;
    int columnIndex(int column) const;

    // ����������� �������
    int size() const;
    // ����� �� �����
    void print() const;
    void printEndl() const;

    // �������� ������ � ������� �� ��������
    void removeRowColumn(int row, int column);

    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;

private:
    // ������������� "��������" ������� ����� � ��������
    void initRowsColumns();

    std::vector<std::vector<double>> _items;
    // "��������" ������ �����
    std::vector<int> _rows;
    // "��������" ������ ��������
    std::vector<int> _columns;
};


