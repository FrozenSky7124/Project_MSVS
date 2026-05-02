#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>

typedef struct _USERDATA_
{
    QString email;
    QString user;
    QString passwd;
} UserData;

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

signals:
    void SIG_UserDataSubmitted(const UserData & userdata);

private:
    Ui::Register *ui;

    QLineEdit* lineEditEmail;
    QLineEdit* lineEditUser;
    QLineEdit* lineEditPasswd;

    UserData getUserData() const;

private slots:
    void on_btnRegister_clicked();
    void on_btnClear_clicked();
};

#endif // REGISTER_H
