#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class loginDlg : public QDialog
{
    Q_OBJECT
public:
    loginDlg();

private:
    QLabel *label0, *label1, *label2, *label3;
    QLineEdit *lineEditUserID, *lineEditPasswd, *lineEditDBName, *lineEditHostIP;
    QPushButton *loginBtn, *logoutBtn;

public:
    QString userid;
    QString passwd;
    QString dbname;
    QString hostip;
    bool islogin;

private slots:
    void loginBtnOnclick();
    void logoutBtnOnclick();
};

#endif // LOGINDLG_H
