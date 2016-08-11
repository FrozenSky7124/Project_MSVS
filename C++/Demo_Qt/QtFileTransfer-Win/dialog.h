#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTcpSocket>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    QLabel *label1, *label2;
    QLineEdit *edit1, *edit2;
    QPushButton *btn1;
    QTcpSocket *tcpsocket;

    QString filename;


private slots:
    void on_clicked1();
    void dataReceived();
};

#endif // DIALOG_H
