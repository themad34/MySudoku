#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QVector>
#include <QIntValidator>
#include "ortools/sat/cp_model.h"
#include <vector>
#include "sudokucell.h"
using namespace std;
using namespace operations_research;
using namespace operations_research::sat;

struct SolverOutput {
    int solCount;
    CpSolverResponse response;
    vector<vector<IntVar>> vars;
};


class SudokuBoard : public QWidget {
    Q_OBJECT

public:
    SudokuBoard(QWidget *parent = nullptr);
    bool isValid(SudokuCell *cell) const;
    void updateBoard() const;
    void checker();
    void completeGrid();

private:
    int size;
    int subsize;
    QGridLayout *gridLayout;
    QVector<QVector<SudokuCell*>> cells;
    QList<SudokuCell*> invalidCells;

    SolverOutput solve(vector<vector<int>> grid, bool allSolutions = false);
    
    void prefillBoard(int n);

private slots:
    void highlightCells(QString value);
    void moveCellFocus(int rowOffset, int colOffset);
};

#endif // SUDOKUBOARD_H
