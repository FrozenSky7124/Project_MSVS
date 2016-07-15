#include "mainwindow.h"
#include <QMessageBox>

#define Author "FrozenSky"
#define StartColor #66CCFF
#define EndColor #FFFFFF

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //MainWindow UI
    this->setWindowTitle("PostColorfull");

    layout = new QGridLayout(this);
    OldTextEdit = new QTextEdit(this);
    NewTextEdit = new QTextEdit(this);
    ConvButton = new QPushButton(this);

    ConvButton->setText("Convert");

    layout->addWidget(OldTextEdit, 0, 0, 1, 6);
    layout->addWidget(ConvButton, 0, 6, 1, 1);
    layout->addWidget(NewTextEdit, 0, 7, 1, 6);

    //Slots Function
    connect(ConvButton, SIGNAL(clicked(bool)), this, SLOT(onConvertBtn()));
}

Widget::~Widget()
{
    //
}

void Widget::onConvertBtn()
{
    //
    QString oldStr;
    QString newStr;

    oldStr = OldTextEdit->toPlainText();

    QChar cChar;
    cChar = oldStr.at(3);
    newStr = QString(cChar);

    NewTextEdit->append(newStr);
    //QMessageBox::information(this, "Main", oldStr);
}
