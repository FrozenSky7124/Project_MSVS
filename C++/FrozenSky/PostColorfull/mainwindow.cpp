#include "mainwindow.h"
#include <QMessageBox>
#include <QFont>

#define Author "FrozenSky"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //MainWindow UI
    this->setWindowTitle("PostColorfull");
    this->setWindowIcon(QIcon(":/icon.ico"));

    layout = new QGridLayout(this);
    OldTextEdit = new QTextEdit(this);
    NewTextEdit = new QTextEdit(this);
    ConvButton = new QPushButton(this);

    ConvButton->setText("Convert && Copy");
    OldTextEdit->setFont(QFont("微软雅黑", 12));

//    layout->addWidget(OldTextEdit, 0, 0, 1, 6);
//    layout->addWidget(ConvButton, 0, 6, 1, 1);
//    layout->addWidget(NewTextEdit, 0, 7, 1, 6);

    layout->addWidget(OldTextEdit, 0, 0, 7, 10);
    layout->addWidget(ConvButton, 8, 4, 1, 2);
    layout->addWidget(NewTextEdit, 9, 0, 4, 10);

    //Slots Function
    connect(ConvButton, SIGNAL(clicked(bool)), this, SLOT(onConvertBtn()));
}

Widget::~Widget()
{
    //
}

void Widget::onConvertBtn()
{
    //染色函数
    NewTextEdit->clear();
    /*
    int arrayColor[18] =
    {
        0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000,
        0xFF3300, 0xCC3300, 0xFF6600, 0xFF9900, 0x009900, 0x339900,
        0x00CC00, 0x33CC00, 0x66CC00, 0x00FF00, 0x66FF00, 0xCCFF00,
    };   
    int arrayColor_Backup[18] =
    {
        0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000,
        0xFF3300, 0xCC3300, 0xFF6600, 0xFF9900, 0x009900, 0x339900,
        0x00CC00, 0x33CC00, 0x66CC00, 0x00FF00, 0x66FF00, 0xCCFF00,
    };
    */
    int arrayColor[18] =
    {
        0x000066, 0x330066, 0x660066, 0x990066, 0xCC0066, 0xFF0066,
        0xFF3300, 0xCC3300, 0xFF6600, 0xFF9900, 0x009900, 0x339900,
        0x00CC00, 0x33CC00, 0x66CC00, 0x00FF00, 0x66FF00, 0xCCFF00,
    };
    int arrayColor_Backup[18] =
    {
        0x000066, 0x330066, 0x660066, 0x990066, 0xCC0066, 0xFF0066,
        0xFF3300, 0xCC3300, 0xFF6600, 0xFF9900, 0x009900, 0x339900,
        0x00CC00, 0x33CC00, 0x66CC00, 0x00FF00, 0x66FF00, 0xCCFF00,
    };

    QString oldStr;
    QString newStr;
    int n = 0;
    oldStr = OldTextEdit->toPlainText();

    int oldStrLen = oldStr.length();
    for (int i = 0; i < oldStrLen; i++)
    {
        QChar tmpChar;
        QString tmpStr;
        tmpChar = oldStr.at(i); //读取待转换字符串第i个字符
        if (QString(tmpChar) == "\n") //换行时颜色标识n+1
        {
            n++;
            if(n >= 18) //超出色彩循环范围则重置颜色标识n和基础色数组
            {
                n = 0;
                memcpy_s(&arrayColor, 18 * sizeof(int), &arrayColor_Backup, 18 * sizeof(int));
            }
            tmpStr = QString(tmpChar);
        }
        else
        {
            tmpStr.sprintf("[color=%06x]%s[/color]", arrayColor[n] += 0x000003, QString(tmpChar).toStdString().data());
        }
        newStr += tmpStr;
    }

    NewTextEdit->append(newStr);
    NewTextEdit->selectAll();
    NewTextEdit->copy();
    //QMessageBox::information(this, "Main", oldStr);
}
