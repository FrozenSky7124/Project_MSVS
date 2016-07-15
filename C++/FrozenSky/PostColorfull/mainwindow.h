#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QGridLayout *layout;
    QTextEdit *OldTextEdit;
    QTextEdit *NewTextEdit;
    QPushButton *ConvButton;

private slots:
    void onConvertBtn();
};

#endif // MAINWINDOW_H
