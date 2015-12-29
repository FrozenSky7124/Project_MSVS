#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    btn1 = new QPushButton(this);
    btn2 = new QPushButton(this);
    edit1 = new QLineEdit(this);
    edit2 = new QLineEdit(this);
    label1 = new QLabel(this);
    timer1 = new QTimer(this);
    btn1->setText("Sum");
    btn2->setText("New Windows");
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(edit1,0,0);
    layout->addWidget(edit2,0,1);
    layout->addWidget(btn1,1,0);
    layout->addWidget(label1,1,1);
    layout->addWidget(btn2,3,0);

    connect(btn1, SIGNAL(clicked()), this, SLOT(add()));//关联点击事件
    connect(btn2,SIGNAL(clicked()),this,SLOT(showAbort()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(onTime()));//关联时间事件
}

Dialog::~Dialog()
{

}

void Dialog::add()
{
    QString s1 = edit1->text();
    QString s2 = edit2->text();
    //s1.toStdString().data();//QString向标准字符串转换
    //s2.toStdString().data();//QString向标准字符串转换
    QString s3 = s1 + " + " + s2 + " = ";
    bool ok = false;
    int a = s1.toInt(&ok,10);
    if (ok == false)
        return;
    int b = s2.toInt(&ok,10);
    if (ok == false)
        return;
    int c = a + b;
    s3 = s3 + QString::number(c);
    label1->setText(s3);
    timer1->start(1000);//设置倒计时
}

void Dialog::onTime()
{
    QDateTime *dt1 = new QDateTime(QDateTime::currentDateTime());
    QString s;
    s.sprintf("%d:%d:%d",dt1->time().hour(),dt1->time().minute(),dt1->time().second());
    setWindowTitle(s);
}

void Dialog::showAbort()
{
    QDialog abort;
    abort.resize(320,240);
    abort.exec();
}
