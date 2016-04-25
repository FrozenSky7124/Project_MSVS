#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
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
    //天涯明月刀OL
    QPushButton *openDBBtn;
    QPushButton *updateDBBtn;
    QPushButton *createTableBtn;
    QPushButton *importFileBtn;
    QPushButton *selectTableBtn;
    //剑侠情缘网络版三
    QPushButton *openDBBtn1;
    QPushButton *updateDBBtn1;
    QPushButton *createTableBtn1;
    QPushButton *importFileBtn1;
    QPushButton *selectTableBtn1;

    QTextEdit *mainTextEdit;
    void insertToDB(QString id, QString pyquestion, QString question, QString answer, QString rightnum);
    void insertToDB1(QString pyquestion, QString question, QString answer);

private slots:
    void showDBDrivers();
    bool openDB();
    bool createTableQuery();
    void importFile();
    void selectTableQuery();
    void updateDB();
    void updateSql(int idStart, int idEnd, int newid);

    bool openDB1();
    bool createTableQuery1();
    void importFile1();
//    void updateDB1();
    void selectTableQuery1();
};

#endif // MAINWINDOW_H
