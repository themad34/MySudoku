#include <QIntValidator>
#include <QFont>
#include <QPalette>
#include <QVector>
#include <QApplication>
#include "sudokuboard.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
using namespace std;
#include <algorithm>
#include <iterator>
#include <random>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QEvent>
#include "sudokucell.h"


#include "ortools/sat/cp_model.h"
using namespace operations_research;
using namespace operations_research::sat;

SudokuBoard::SudokuBoard(QWidget *parent)
    : QWidget(parent), size(9), cells(size, QVector<SudokuCell*>(size, nullptr)) {
    //setLayout(gridLayout);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout();


    subsize = sqrt(size);
    
    QString color1 = "#333333";
    QString color2 = "#444444";

    
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            cells[row][col] = new SudokuCell(row, col, this);
            cells[row][col]->setMaxLength(2);
            cells[row][col]->setAlignment(Qt::AlignCenter);
            cells[row][col]->setValidator(new QIntValidator(1, size, this));
            cells[row][col]->setFixedSize(50, 50); 

            QString baseStyle;
            if ((row / subsize + col / subsize) % 2 == 0) {
                cells[row][col]->defaultBackgroundColor = color1;
                cells[row][col]->setBackgroundColor(color1);
            } else {
                cells[row][col]->defaultBackgroundColor = color2;
                cells[row][col]->setBackgroundColor(color2);
            }


            gridLayout->addWidget(cells[row][col], row, col);
            
            connect(cells[row][col], &SudokuCell::moveFocus, this, &SudokuBoard::moveCellFocus);
            connect(cells[row][col], &SudokuCell::focusChanged, this, &SudokuBoard::highlightCells);
            connect(cells[row][col], &QLineEdit::textChanged, this, &SudokuBoard::highlightCells);
            connect(cells[row][col], &QLineEdit::textChanged, [this]() {
                
                updateBoard();
                checker();
            });


            
        }
    }


    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(10, 10, 10, 10);

    QPushButton *solveButton = new QPushButton("Solve!", this);
    connect(solveButton, &QPushButton::clicked, this, &SudokuBoard::completeGrid);
    
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(solveButton);

    setLayout(mainLayout);


    //Pré-remplissage de la grille
    prefillBoard(50);

}

bool SudokuBoard::isValid(SudokuCell *cell) const {
    int row = cell->getRow();
    int col = cell->getCol();
    QString value = cell->text();


    //Vérifie la ligne
    for (int c = 0; c < size; ++c) {
        if (c != col) {
            QString text = cells[row][c]->text();
            if (!text.isEmpty() && text == value) {
                
                return false;
   
            }
        }
    }

    //Vérifie la colonne
    for (int r = 0; r < size; ++r) {
        if (r != row) {
            QString text = cells[r][col]->text();
            if (!text.isEmpty() && text == value) {
                
                return false;
                
            }
        }
    }

    //Vérifie le carré
    int startRow = (row / subsize) * subsize;
    int startCol = (col / subsize) * subsize;
    for (int r = startRow; r < startRow + subsize; ++r) {
        for (int c = startCol; c < startCol + subsize; ++c) {
            if (r != row || c != col) {
                QString text = cells[r][c]->text();
                if (!text.isEmpty() && text == value) {
                
                    return false;
                    
                }
            }
        }
    }


    return true;
}

//Mise à jour des couleurs des valeurs (rouge si incorrecte)
void SudokuBoard::updateBoard() const {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!cells[i][j]->text().isEmpty()) {
                if (isValid(cells[i][j])) {
                    cells[i][j]->setFontColor();
                } else {
                    cells[i][j]->setFontColor("red");
                }
            }
        }
    }
}

//Vérifie si on a résolu la grille
void SudokuBoard::checker() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (cells[i][j]->text().isEmpty()) return;
            if (!isValid(cells[i][j])) return;
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cells[i][j]->setFontColor("green");
        }
    }
    


}

SolverOutput SudokuBoard::solve(vector<vector<int>> grid, bool allSolutions) {
    CpModelBuilder cp_model;

    //Créer les variables pour la grille
    vector<vector<IntVar>> vars(size, vector<IntVar>(size));

    //Ajouter les variables au modèle
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            vars[row][col] = cp_model.NewIntVar(Domain(1, size));
        }
    }

    //Ajouter les contraintes pour les lignes
    for (int row = 0; row < size; ++row) {
        vector<IntVar> row_vars;
        for (int col = 0; col < size; ++col) {
            row_vars.push_back(vars[row][col]);
        }
        cp_model.AddAllDifferent(row_vars);
    }

    //Ajouter les contraintes pour les colonnes
    for (int col = 0; col < size; ++col) {
        vector<IntVar> col_vars;
        for (int row = 0; row < size; ++row) {
            col_vars.push_back(vars[row][col]);
        }
        cp_model.AddAllDifferent(col_vars);
    }

    //Ajouter les contraintes pour les sous-grilles
    
    for (int r = 0; r < size; r += subsize) {
        for (int c = 0; c < size; c += subsize) {
            vector<IntVar> block_vars;
            for (int i = 0; i < subsize; ++i) {
                for (int j = 0; j < subsize; ++j) {
                    block_vars.push_back(vars[r + i][c + j]);
                }
            }
            cp_model.AddAllDifferent(block_vars);
        }
    }

    //Lire la grille et fixer les valeurs des variables pour les cellules read-only
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            if (grid[row][col]) {
                cp_model.AddEquality(vars[row][col], grid[row][col]);
            }
        }
    }
    //Calculer toutes les solutions si argument allSolutions = vrai, sinon une seule
    if(allSolutions) {
        Model model;
        int num_solutions = 0;
        model.Add(NewFeasibleSolutionObserver([&](const CpSolverResponse& r) {num_solutions++;
        }));

        SatParameters parameters;
        parameters.set_enumerate_all_solutions(true);
        
        model.Add(NewSatParameters(parameters));
        CpSolverResponse response = SolveCpModel(cp_model.Build(), &model);

        return {num_solutions, response, vars};

    }
    else {
        CpSolverResponse response = Solve(cp_model.Build());

        return {1, response, vars};
    }

}

void SudokuBoard::completeGrid() {

    vector<vector<int>> grid(size, vector<int>(size));

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            
            if (cells[row][col]->isReadOnly()) {
                grid[row][col] = cells[row][col]->text().toInt();
            }      
        }
    }


    SolverOutput sol = solve(grid);
    


    if (sol.response.status() == CpSolverStatus::OPTIMAL || sol.response.status() == CpSolverStatus::FEASIBLE) {
        
        //Écrire la solution dans la grille
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                int value = SolutionIntegerValue(sol.response, sol.vars[row][col]);
                cells[row][col]->setText(QString::number(value));
            }
        }
    } else {
        cerr << "No solution found" << endl;
    }
}

void SudokuBoard::prefillBoard(int nFilled) {

    vector<vector<int>> grid(size, vector<int>(size));

    vector<int> values1(size);
    vector<int> values2(size);

    for (int i = 0; i < size; ++i) {
        values1[i] = i + 1;
        values2[i] = i + 1;
    }
    
    srand(time(nullptr));
    shuffle(begin(values1), end(values1), mt19937{random_device{}()});
    shuffle(begin(values2), end(values2), mt19937{random_device{}()});

    for (int row = 0; row < subsize; ++row) {
        for (int col = 0; col  < subsize; ++col) {
            grid[row][col] = values1[row * subsize + col];
        }
    }

    for (int row = size - subsize; row < size; ++row) {
        for (int col = size - subsize; col < size; ++col) {
            grid[row][col] = values2[(row - size + subsize) * subsize + (col - size + subsize)];
        }
    }

    SolverOutput s = solve(grid);

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            grid[row][col] = SolutionIntegerValue(s.response, s.vars[row][col]);
        }
        
    }

    int toRemove = size * size - nFilled;
    int r1, r2;

    for (int n = 0; n < toRemove; ++n) {
        do{
            r1 = (rand() % size);
            r2 = (rand() % size);
        }
        while(!grid[r1][r2]);

        int tmp = grid[r1][r2];
        grid[r1][r2] = 0;

        s = solve(grid, true);
        if (s.solCount>1) {
            n--;
            grid[r1][r2] = tmp;
        }
    }


    QFont boldFont;
    boldFont.setBold(true);


    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            if (grid[row][col]) {
                cells[row][col]->setText(QString::number(grid[row][col]));
                cells[row][col]->setReadOnly(true);
                cells[row][col]->setFont(boldFont);
            }
        }
    }
}

void SudokuBoard::highlightCells(QString value) {
    int intValue = value.toInt();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (intValue > 0 && cells[i][j]->text().toInt() == intValue) {
                cells[i][j]->setBackgroundColor("#111111");
            } else {
                cells[i][j]->setBackgroundColor();
            }
        }
    }
}

void SudokuBoard::moveCellFocus(int rowOffset, int colOffset) {
    QWidget *focusedWidget = QApplication::focusWidget();
    SudokuCell *currentCell = qobject_cast<SudokuCell *>(focusedWidget);
    if (focusedWidget) {
        int row = currentCell->getRow();
        int col = currentCell->getCol();
        int newRow = (row + rowOffset + size) % size;
        int newCol = (col + colOffset + size) % size;
        cells[newRow][newCol]->setFocus();
    }
}