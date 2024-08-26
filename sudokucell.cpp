#include "sudokucell.h"

SudokuCell::SudokuCell(int i, int j, QWidget *parent)
    : row(i), col(j), QLineEdit(parent) {
}


int SudokuCell::getRow() const {
    return row;
}

int SudokuCell::getCol() const {
    return col;
}


void SudokuCell::focusInEvent(QFocusEvent *event) {
        emit focusChanged(this->text());
        QLineEdit::focusInEvent(event);
    }

void SudokuCell::focusOutEvent(QFocusEvent *event) {
        emit focusChanged("");
        QLineEdit::focusOutEvent(event);
    }

void SudokuCell::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:
        emit moveFocus(-1, 0);
        break;
    case Qt::Key_Down:
        emit moveFocus(1, 0);
        break;
    case Qt::Key_Left:
        emit moveFocus(0, -1);
        break;
    case Qt::Key_Right:
        emit moveFocus(0, 1);
        break;
    default:
        QLineEdit::keyPressEvent(event);
    }
}

void SudokuCell::setBackgroundColor(QString color) {
    if (color == "") {
        this->setStyleSheet("background-color: " + defaultBackgroundColor + "; color:" + fontColor + "; font-size: " + QString::number(fontSize) + "px;");
        backgroundColor = defaultBackgroundColor;
    } else {
        this->setStyleSheet("background-color: " + color + "; color:" + fontColor + "; font-size: " + QString::number(fontSize) + "px;");
        backgroundColor = color;
    }
}

void SudokuCell::setFontColor(QString color) {
    if (color == "") {
        this->setStyleSheet("background-color: " + backgroundColor + "; color:" + defaultFontColor + "; font-size: " + QString::number(fontSize) + "px;");
        fontColor = defaultFontColor;
    } else {
        this->setStyleSheet("background-color: " + backgroundColor + "; color:" + color + "; font-size: " + QString::number(fontSize) + "px;");
        fontColor = color;
    }
}

void SudokuCell::setFontSize(int size) {
    if (size == -1) {
        this->setStyleSheet("background-color: " + backgroundColor + "; color:" + fontColor + "; font-size: " + QString::number(defaultFontSize) + "px;");
        fontSize = defaultFontSize;
    } else {
        this->setStyleSheet("background-color: " + backgroundColor + "; color:" + fontColor + "; font-size: " + QString::number(size) + "px;");
        fontSize = size;
    }
}

