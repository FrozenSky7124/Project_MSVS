#include "register.h"
#include "ui_ui_register.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

Register::Register(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Register"));
    this->setMinimumSize(400, 500);
    this->setMaximumSize(400, 500);
    
    auto layoutEmail = new QHBoxLayout();
    auto labelEmail = new QLabel();
    labelEmail->setText(tr("Email:"));
    labelEmail->setMinimumWidth(60);
    auto lineEditEmail = new QLineEdit();
    layoutEmail->addWidget(labelEmail);
    layoutEmail->addWidget(lineEditEmail);

    auto layoutUser = new QHBoxLayout();
    auto labelUser = new QLabel();
    labelUser->setText(tr("UserName:"));
    labelUser->setMinimumWidth(60);
    auto lineEditUser = new QLineEdit();
    layoutUser->addWidget(labelUser);
    layoutUser->addWidget(lineEditUser);

    auto layoutPasswd = new QHBoxLayout();
    auto labelPasswd = new QLabel();
    labelPasswd->setText(tr("Password:"));
    labelPasswd->setMinimumWidth(60);
    auto lineEditPasswd = new QLineEdit();
    layoutPasswd->addWidget(labelPasswd);
    layoutPasswd->addWidget(lineEditPasswd);
    
    auto layoutAct = new QHBoxLayout();
    auto btnRegister = new QPushButton();
    btnRegister->setText(tr("REGISTER"));
    auto btnClear = new QPushButton();
    btnClear->setText(tr("CLEAR"));
    layoutAct->addWidget(btnRegister);
    layoutAct->addWidget(btnClear);

    auto layoutVMain = new QVBoxLayout(); // Main Layout
    auto spacerV1 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layoutVMain->addItem(spacerV1);
    layoutVMain->addLayout(layoutEmail);
    layoutVMain->addLayout(layoutUser);
    layoutVMain->addLayout(layoutPasswd);
    layoutVMain->addLayout(layoutAct);
    auto spacerV2 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layoutVMain->addItem(spacerV2);

    this->setLayout(layoutVMain);

    this->lineEditEmail = lineEditEmail;
    this->lineEditUser = lineEditUser;
    this->lineEditPasswd = lineEditPasswd;

    QObject::connect(btnRegister, &QPushButton::clicked, this, &Register::on_btnRegister_clicked);
    QObject::connect(btnClear, &QPushButton::clicked, this, &Register::on_btnClear_clicked);
}

Register::~Register()
{
    delete ui;
}

UserData Register::getUserData() const
{
    UserData userdata;
    userdata.email = lineEditEmail->text();
    userdata.user = lineEditUser->text();
    userdata.passwd = lineEditPasswd->text();
    return userdata;
}

void Register::on_btnRegister_clicked()
{
    UserData userData = getUserData();
    if (userData.email.isEmpty() || userData.user.isEmpty() || userData.passwd.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("All fields are required!"));
        return;
    }
    emit SIG_UserDataSubmitted(userData);
    this->accept();
}

void Register::on_btnClear_clicked()
{
    lineEditEmail->clear();
    lineEditUser->clear();
    lineEditPasswd->clear();
}
