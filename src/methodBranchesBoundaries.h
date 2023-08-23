#pragma once

#include <list>
#include <utility>
#include <memory>
#include <cfloat>
#include <mutex>
#include <atomic>
#include <stddef.h>
#include <stdint.h>
#include "matrix.h"


class LittleSolver {
public:

    // �� ���� ������� ���������� � ��������� ������� �������
    LittleSolver(Matrix& m, double record = DBL_MAX);
    ~LittleSolver();

    // �������� �����
    void solve();

    // �������� �������
    std::list<int> getSolution() const;
  
    // �������� ������
    double getRecord() const;
    // ���� �� ������� �������, �� ����������� �������� �������
    bool isSolved() const;

    // �� ������ ���� ����������
    LittleSolver(const LittleSolver&) = delete;
    LittleSolver& operator=(const LittleSolver&) = delete;

private:
    // �������� ����������� ������� ������ ����
    // m - ������� ������� ����������
    // arcs - ������� ��������� ����
    // bottomLimit - ������� ������ �������
    void handleMatrix(Matrix& m, const std::list<std::pair<int, int>>& arcs, double bottomLimit);
    // ��������� ����� ������ �����
    double cost(const std::list<std::pair<int, int>>& arcs);
    // �������� ������������ ������� � �����������
    void candidateSolution(const std::list<std::pair<int, int>>& arcs);
    // ���������� ����������� ������������� � �������
    // ���������� ��� ��������� ��������������� ������
    void addInfinity(Matrix& m);
    // ���������� ��� ���������, ����� � ������ ������
    // � ������ ������� ���� �������������.
    // ���������� ��������, �� ������� ���������� ������ �������
    double subtractFromMatrix(Matrix& m) const;
    // ����� ������� ������������� � ������������� ��������������
    std::list<std::pair<int, int>>  findBestZeros(Matrix& matrix) const;
    // ��������� ������������ ��� �������� (r, c)
    // r - row; c - column
    static double getCoefficient(Matrix& m, int r, int c);

    // �������� ������� ����������
    Matrix _sourceMatrix;
    // ������, �� �� ����� ������� ����, �� �� ������� �������
    std::atomic<double> _record;
    // ������ �������
    std::list<std::pair<int, int>> _arcs;
    // �������� �������
    std::list<int> _solution;
    // ��������� ������������� ������ �����
    std::list<std::pair<int, int>> _lastStep;
    
    // ��������, ����������� �� �������������
    double _infinity;
};