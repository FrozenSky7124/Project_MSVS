#include "QMYWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMYWidget w;
    w.show();
    return a.exec();
}
