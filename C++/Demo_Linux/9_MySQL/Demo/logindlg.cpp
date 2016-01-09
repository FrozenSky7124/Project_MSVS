#include "logindlg.h"
#include <QGridLayout>
#include <QMessageBox>

//loginDlg::loginDlg(QWidget *parent) : QDialog(parent)
loginDlg::loginDlg()
{
    islogin = false;

    label0 = new QLabel;
    label0->setText(tr("用户名"));

    label1 = new QLabel;
    label1->setText(tr("密码"));

    label2 = new QLabel;
    label2->setText(tr("数据库名称"));

    label3 = new QLabel;
    label3->setText(tr("服务器IP"));

    lineEditUserID = new QLineEdit;
    lineEditPasswd = new QLineEdit;
    lineEditPasswd->setEchoMode(QLineEdit::Password);
    lineEditDBName = new QLineEdit;
    lineEditHostIP = new QLineEdit;

    lineEditUserID->setText("dbatest");
    lineEditDBName->setText("dbtest");
    lineEditHostIP->setText("192.168.1.112");

    loginBtn = new QPushButton;
    logoutBtn = new QPushButton;
    loginBtn->setText(tr("登录"));
    logoutBtn->setText(tr("取消"));

    QGridLayout * layout1 = new QGridLayout(this);
    layout1->addWidget(label0, 0, 0);
    layout1->addWidget(label1, 1, 0);
    layout1->addWidget(label2, 2, 0);
    layout1->addWidget(label3, 3, 0);
    layout1->addWidget(lineEditUserID, 0, 1);
    layout1->addWidget(lineEditPasswd, 1, 1);
    layout1->addWidget(lineEditDBName, 2, 1);
    layout1->addWidget(lineEditHostIP, 3, 1);
    layout1->addWidget(loginBtn, 4, 0);
    layout1->addWidget(logoutBtn, 4, 1);

    layout1->setColumnStretch(0, 1);//设置GRIDLAYOUT列比例
    layout1->setColumnStretch(1, 1);//设置GRIDLAYOUT列比例
    layout1->setMargin(15);
    layout1->setSpacing(10);
    layout1->setSizeConstraint(QLayout::SetFixedSize);//设置无法更改大小
    //图标设置
    this->setWindowIcon(QIcon("img_Icon.jpg"));
    //背景图片设置
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("img_Dlg.jpg")));
    this->setPalette(palette);

    this->setWindowTitle("登录");

    connect(loginBtn, SIGNAL(clicked()), this, SLOT(loginBtnOnclick()));
    connect(logoutBtn, SIGNAL(clicked()), this, SLOT(logoutBtnOnclick()));
}

void loginDlg::loginBtnOnclick()
{
    this->userid = lineEditUserID->text();
    this->passwd = lineEditPasswd->text();
    this->dbname = lineEditDBName->text();
    this->hostip = lineEditHostIP->text();
    islogin = true;
    this->close();
}

void loginDlg::logoutBtnOnclick()
{
    this->close();
}

