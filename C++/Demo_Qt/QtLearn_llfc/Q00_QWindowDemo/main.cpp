#include "QWindowDemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWindowDemo w;
    w.show();
    return a.exec();
}
