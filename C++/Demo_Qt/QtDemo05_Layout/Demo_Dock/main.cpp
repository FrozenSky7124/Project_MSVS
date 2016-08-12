#include "dockwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DockWindow w;
    w.show();
    return a.exec();
}
