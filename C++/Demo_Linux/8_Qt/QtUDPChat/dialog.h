#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QUdpSocket>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    QLabel *label1, *label2, *label3, *label4;
    QLineEdit *edit1, *edit2, *edit3, *edit4;
    QPushButton *btn1, *btn2;
    QUdpSocket *udpsocket;


private slots:
    void on_clicked1();
    void on_clicked2();
    void dataReceived();
};

#endif // DIALOG_H
