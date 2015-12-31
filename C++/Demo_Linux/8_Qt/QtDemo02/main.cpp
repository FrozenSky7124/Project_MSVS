#include "widget.h"
#include "drawer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    drawer d;
    d.resize(300, 600);
    d.show();

    return a.exec();
}
