#include "qtdemo08.h"
#include <QApplication>
#include <QUiLoader>
#include <QFile>
#include <QWidget>
#include <QDebug>
#include <QDir>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << QDir::currentPath();
    QUiLoader uiLoader;
    QFile uiFile("../QtDemo08_QUiLoader/qtdemo08.ui");
    QWidget *w = uiLoader.load(&uiFile);
    if (w)
    {
        QPushButton *btn = w->findChild<QPushButton*>("pushButton");
        btn->setText("UI Loaded!");
        w->show();
    }

    return a.exec();
}
