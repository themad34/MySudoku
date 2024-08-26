#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QEvent>

class SudokuCell : public QLineEdit {
    Q_OBJECT

public:
    explicit SudokuCell(int i, int j, QWidget *parent = nullptr);
    
    void setBackgroundColor(QString color = "");
    void setFontColor(QString color = "");
    void setFontSize(int size = -1);

    QString backgroundColor = "#333333";
    QString fontColor = "white";
    int fontSize = 18;
    
    QString defaultBackgroundColor = "#333333";
    QString defaultFontColor = "white";
    int defaultFontSize = 18;

    int getRow() const;
    int getCol() const;

signals:
    void focusChanged(QString value);
    void moveFocus(int rowOffset, int colOffset);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    int row;
    int col;
  
};

#endif
