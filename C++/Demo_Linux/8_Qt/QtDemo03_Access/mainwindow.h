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
    QPushButton *showDBDriversBtn;
    QPushButton *openDBBtn;
    QPushButton *createTableBtn;
    QPushButton *importFileBtn;
    QPushButton *selectTableBtn;
    QTextEdit *mainTextEdit;
    void insertToDB(QString id, QString pyquestion, QString question, QString answer);

private slots:
    void showDBDrivers();
    bool openDB();
    bool createTableQuery();
    void importFile();
    void selectTableQuery();
};

#endif // MAINWINDOW_H
