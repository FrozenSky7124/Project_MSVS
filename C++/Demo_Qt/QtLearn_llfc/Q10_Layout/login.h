#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QVector>
#include <QDebug>
#include "register.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login() override;

private slots:
    void on_btnRegister_clicked();
    void on_UserRegister(const UserData & userdata);

private:
    Ui::Login *ui;

    QVector<UserData> vUserList;
};
#endif // LOGIN_H
