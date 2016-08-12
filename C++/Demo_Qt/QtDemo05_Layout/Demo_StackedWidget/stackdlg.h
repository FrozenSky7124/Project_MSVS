#ifndef STACKDLG_H
#define STACKDLG_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>

class StackDlg : public QDialog
{
    Q_OBJECT

public:
    StackDlg(QWidget *parent = 0);
    ~StackDlg();
private:
    QListWidget *list;
    QStackedWidget *stack;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
};

#endif // STACKDLG_H
