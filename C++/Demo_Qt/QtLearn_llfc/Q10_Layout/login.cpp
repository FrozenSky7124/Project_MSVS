#include "login.h"
#include "./ui_ui_login.h"
#include "register.h"

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
    winRegister.exec();
}

