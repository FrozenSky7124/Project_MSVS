#include "mainWin.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWin w;
    w.show();
    return a.exec();
}
