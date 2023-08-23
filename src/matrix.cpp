#include "matrix.h"

Matrix::Matrix(int size, double values)
    :_items(size, std::vector<double>(size, values)), _rows(size), _columns(size) {
    initRowsColumns();
}


double& Matrix::item(int row, int column) {
    return _items[row][column];
}

double& Matrix::operator()(int row, int column) {
    return item(row, column);
}

int Matrix::rowIndex(int row) const {
    return _rows[row];
}

int Matrix::columnIndex(int column) const {
    return _columns[column];
}

void Matrix::removeRowColumn(int row, int column) {
    // удаление индексов
    _rows.erase(_rows.begin() + row);
    _columns.erase(_columns.begin() + column);
    // удаление строки
    _items.erase(_items.begin() + row);
    // удаление столбца
    for (int i = 0; i < _items.size(); i++)
        _items[i].erase(_items[i].begin() + column);
}


int Matrix::size() const {
    return _items.size();
}


void Matrix::print() const {
    printf("         ");
    for (auto iter = _columns.cbegin(); iter != _columns.cend(); ++iter)
        printf("%8u ", *iter);
    puts("");
    for (int i = 0; i < _items.size(); i++) {
        printf("%8u ", _rows[i]);
        for (int j = 0; j < _items.size(); j++) {
            printf("%8.2f ", _items[i][j]);
        }
        puts("");
    }
}


void Matrix::printEndl() const {
    print();
    puts("");
}


void Matrix::initRowsColumns() {
    for (int i = 0; i < _rows.size(); i++)
        _rows[i] = _columns[i] = i;
}

