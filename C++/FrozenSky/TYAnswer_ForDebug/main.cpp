#include "mainWin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("icon.ico"));
    Widget w;
    w.resize(650,420);
    w.show();

    return a.exec();
}
