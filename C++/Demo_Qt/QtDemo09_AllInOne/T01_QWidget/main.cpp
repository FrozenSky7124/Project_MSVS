#include "T01_Widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    T01_Widget w;
    w.show();
    return a.exec();
}
