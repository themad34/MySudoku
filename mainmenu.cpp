#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{

    setMinimumSize(400, 400);
    layout = new QVBoxLayout(this);

    sizeBox = new QComboBox(this);
    sizeBox->addItem("4*4");
    sizeBox->addItem("9*9");
    sizeBox->addItem("16*16");
    sizeBox->setCurrentIndex(1);

    fillBox = new QSpinBox(this);
    fillBox->setSuffix("%");
    fillBox->setRange(50,100);

    
    layout->addWidget(sizeBox);
    layout->addWidget(fillBox);
    //layout->addWidget(button25x25);

    playButton = new QPushButton("Play!", this);
    connect(playButton, &QPushButton::clicked, [this]() {
        playSudoku();
    });
    layout->addWidget(playButton);
}

void MainMenu::playSudoku()
{
    int size = 9;
    QString selectedSize = sizeBox->currentText();
    if (selectedSize == "4*4") size = 4;
    else if (selectedSize == "16*16") size = 16;

    

    int percentage = fillBox->cleanText().toInt();

    SudokuBoard *sudokuBoard = new SudokuBoard(size, percentage);
    sudokuBoard->show();
    this->close();
}
