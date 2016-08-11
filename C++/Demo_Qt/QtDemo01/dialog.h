#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QTimer>
#include <QDateTime>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    QPushButton *btn1, *btn2;
    QLineEdit *edit1, *edit2;
    QLabel *label1;
    QTimer *timer1;

private slots:
    void add();
    void onTime();
    void showAbort();
};

#endif // DIALOG_H
