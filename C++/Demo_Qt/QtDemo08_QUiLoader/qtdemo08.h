#ifndef QTDEMO08_H
#define QTDEMO08_H

#include <QDialog>

namespace Ui {
class QtDemo08;
}

class QtDemo08 : public QDialog
{
    Q_OBJECT

public:
    explicit QtDemo08(QWidget *parent = 0);
    ~QtDemo08();

private:
    Ui::QtDemo08 *ui;
};

#endif // QTDEMO08_H
