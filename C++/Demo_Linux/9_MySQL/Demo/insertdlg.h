#ifndef INSERTDLG_H
#define INSERTDLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QString>

class insertDlg : public QDialog
{
    Q_OBJECT
public:
    insertDlg();
    QString SQL;

    bool isokBtn;

private:
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;
    QLineEdit *leditName;
    QLineEdit *leditSex;
    QLineEdit *leditAge;
    QLineEdit *leditRace;
    QLineEdit *leditSchool;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

private slots:
    void on_okBtn();
    void on_cancelBtn();
};

#endif // INSERTDLG_H
