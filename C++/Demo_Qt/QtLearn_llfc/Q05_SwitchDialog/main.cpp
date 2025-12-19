#include "MainDialog.h"
#include <QApplication>
#include "ChildDialog.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDialog w;
    //w.show();
    ChildDialog childDlg;
    auto res = childDlg.exec();
    if (res == QDialog::Accepted)
    {
        qDebug() << "res = 1";
        w.show();
        a.exec();
    }
    else
    {
        qDebug() << "res = 0";
        return 0;
    }
    //return a.exec();
}
