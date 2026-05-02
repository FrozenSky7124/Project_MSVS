#include "login.h"
#include "./ui_ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnRegister_clicked()
{
    Register winRegister;
    QObject::connect(&winRegister, &Register::SIG_UserDataSubmitted,
                     this, &Login::on_UserRegister);
    winRegister.exec();
}

void Login::on_UserRegister(const UserData &userdata)
{
    vUserList.append(userdata);
    qDebug() << "Registered:" << userdata.user << "Total Users:" << vUserList.size();
}

