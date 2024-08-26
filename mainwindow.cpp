#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), board(new SudokuBoard(this)) {
    setCentralWidget(board);
}

MainWindow::~MainWindow() {
    
}
