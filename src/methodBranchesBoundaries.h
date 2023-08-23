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

    // на вход матрица рассто€ний и требуема€ верхн€€ граница
    LittleSolver(Matrix& m, double record = DBL_MAX);
    ~LittleSolver();

    // основной метод
    void solve();

    // получить решение
    std::list<int> getSolution() const;
  
    // получить рекорд
    double getRecord() const;
    // было ли найдето решение, не превышающее заданную границу
    bool isSolved() const;

    // не должен быть скопирован
    LittleSolver(const LittleSolver&) = delete;
    LittleSolver& operator=(const LittleSolver&) = delete;

private:
    // основна€ рекурсивна€ функци€ поиска пути
    // m - текуща€ матрица стоимостей
    // arcs - текущий найденный путь
    // bottomLimit - текуща€ нижн€€ граница
    void handleMatrix(Matrix& m, const std::list<std::pair<int, int>>& arcs, double bottomLimit);
    // скммарна€ длина набора ребер
    double cost(const std::list<std::pair<int, int>>& arcs);
    // сравнить предложенное решение с оптимальным
    void candidateSolution(const std::list<std::pair<int, int>>& arcs);
    // добавление недостающей бесконечности в матрицу
    // стоимостей дл€ избежани€ преждевременных циклов
    void addInfinity(Matrix& m);
    // произвести р€д вычитаний, чтобы в каждой строке
    // и каждом столбце были бесконечности.
    // возвращает значение, на которое увеличитс€ нижн€€ граница
    double subtractFromMatrix(Matrix& m) const;
    // поиск нулевых коэффициентов с максимальными коэффициентами
    std::list<std::pair<int, int>>  findBestZeros(Matrix& matrix) const;
    // получение коэффициента дл€ элемента (r, c)
    // r - row; c - column
    static double getCoefficient(Matrix& m, int r, int c);

    // исходна€ матрица рассто€ний
    Matrix _sourceMatrix;
    // рекорд, он же длина лучшего пути, он же верхн€€ граница
    std::atomic<double> _record;
    // лучшее решение
    std::list<std::pair<int, int>> _arcs;
    // итоговое решение
    std::list<int> _solution;
    // последний просмотренный список ребер
    std::list<std::pair<int, int>> _lastStep;
    
    // значение, принимаемое за бесконечность
    double _infinity;
};