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
    //剑侠情缘网络版三（重制版）
    QPushButton *openDB_JX3Ex_Btn;
    QPushButton *updateDB_JX3Ex_Btn;
    QPushButton *createTable_JX3Ex_Btn;
    QPushButton *importFile_JX3Ex_Btn1;
    QPushButton *importFile_JX3Ex_Btn2;
    QPushButton *selectTable_JX3Ex_Btn;

    QTextEdit *mainTextEdit;
    void insertToDB(QString id, QString pyquestion, QString question, QString answer, QString rightnum);
    void insertToDB1(QString pyquestion, QString question, QString answer);
    void insertToDB_JX3Ex(int id, QString pyquestion, QString question, QString answer);
    void updatePyToDB_JX3Ex(int id, QString pyquestion);

private slots:
    void showDBDrivers();
    bool openDB();
    bool createTableQuery();
    void importFile();
    void selectTableQuery();
    void updateDB();
    void updateSql_TY(int idStart, int idEnd, int newid);

    bool openDB1();
    bool createTableQuery1();
    void importFile1();
    void updateDB1();
    void selectTableQuery1();
    void changeAnswerDB1(QString question, QString answer);
    void changeQuestionDB1(QString questionOLD, QString questionNEW);

    bool openDB_JX3Ex();
    bool createTable_JX3Ex();
    void importFile_JX3Ex1();
    void importFile_JX3Ex2();
    void updateDB_JX3Ex();
    void selectTable_JX3Ex();

};

#endif // MAINWINDOW_H
