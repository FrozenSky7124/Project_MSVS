#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QFont>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QPushButton *btnOpenDB;
    QTextEdit *mainTextEdit;

private slots:

    //=================================================
    // Func: Show database support drivers
    //=================================================
    void showDBDrivers();

    //=================================================
    // Func: Open Database
    //=================================================
    bool openDB();
};

#endif // MAINWINDOW_H
