#include "T02_Dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    T02_Dialog w;
    w.show();
    return a.exec();
}
