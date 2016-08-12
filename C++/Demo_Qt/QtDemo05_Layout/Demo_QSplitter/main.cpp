#include "mainwindow.h"
#include <QApplication>
#include <QSplitter>
#include <QTextEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("微软雅黑", 12);
    a.setFont(font);
    //
    QSplitter *splitterMain = new QSplitter(Qt::Horizontal, 0);
    QTextEdit *textLeft = new QTextEdit(QObject::tr("Left Widget"), splitterMain);
    splitterMain->setOpaqueResize(true);
    textLeft->setAlignment(Qt::AlignCenter);
    //
    QSplitter *splitterRight = new QSplitter(Qt::Vertical, splitterMain);
    splitterRight->setOpaqueResize(true); //设置拖拽操作实时更新
    QTextEdit *textUp = new QTextEdit(QObject::tr("Top Widget"), splitterRight);
    textUp->setAlignment(Qt::AlignCenter);
    //
    QTextEdit *textBottom = new QTextEdit(QObject::tr("Bottom Widget"), splitterRight);
    textBottom->setAlignment(Qt::AlignCenter);
    splitterMain->setStretchFactor(1, 1); //设置根据窗口大小改变调整内容大小

    splitterMain->setStretchFactor(0, 1);
    splitterMain->setWindowTitle(QObject::tr("Splitter"));
    splitterMain->show();
    return a.exec();
}
