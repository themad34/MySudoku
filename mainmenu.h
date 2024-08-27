#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include "sudokuboard.h"

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);


private:
    QVBoxLayout *layout;

    QComboBox *sizeBox;
    QSpinBox *fillBox;
    QPushButton *playButton;
   
    void playSudoku();
};

#endif // MAINMENU_H
