#include "mainWin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("icon.ico"));
    Widget w;
    w.setWindowFlags(w.windowFlags() | Qt::WindowStaysOnTopHint);
    w.resize(650,420);
    w.show();

    return a.exec();
}
