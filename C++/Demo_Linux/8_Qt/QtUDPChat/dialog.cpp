#include "dialog.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QByteArray>
#include <QHostAddress>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    udpsocket = new QUdpSocket(this);

    setWindowTitle(tr("udp聊天"));//设置窗口标题
    label1 = new QLabel;
    label1->setText(tr("请输入bind端口号"));
    label2 = new QLabel;
    label2->setText(tr("请输入目的IP地址"));
    label3 = new QLabel;
    label3->setText(tr("请输入目的端口号"));
    label4 = new QLabel;
    label4->setText(tr("请输入发送消息"));


    edit1 = new QLineEdit;
    edit2 = new QLineEdit;
    edit3 = new QLineEdit;
    edit4 = new QLineEdit;

    btn1 = new QPushButton;
    btn1->setText(tr("bind"));//设置btn1标题
    btn2 = new QPushButton;
    btn2->setText(tr("发送"));//设置btn2标题

    QGridLayout *layout1 = new QGridLayout(this);

    layout1->addWidget(label1, 0, 0);//加入label1控件
    layout1->addWidget(edit1, 0, 1);//加入edit1控件
    layout1->addWidget(btn1, 1, 0);//加入btn1控件
    layout1->addWidget(label2, 2, 0);//加入label2控件
    layout1->addWidget(edit2, 2, 1);//加入edit2控件
    layout1->addWidget(label3, 3, 0);//加入label3控件
    layout1->addWidget(edit3, 3, 1);//加入label3控件
    layout1->addWidget(label4, 4, 0);//加入label4控件
    layout1->addWidget(edit4, 4, 1);//加入label4控件
    layout1->addWidget(btn2, 5, 0);//加入btn2控件

    connect(btn1, SIGNAL(clicked()), this, SLOT(on_clicked1()));
    connect(btn2, SIGNAL(clicked()), this, SLOT(on_clicked2()));
    connect(udpsocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

}

Dialog::~Dialog()
{

}

void Dialog::on_clicked1()
{
    udpsocket->close();
    udpsocket->bind(edit1->text().toInt());
}

void Dialog::on_clicked2()
{
    QHostAddress addr;
    addr.setAddress(edit2->text());
    int port = edit3->text().toInt();
    udpsocket->writeDatagram(edit4->text().toUtf8(), addr, port);
}

void Dialog::dataReceived()//接收socket消息的槽函数
{
    QHostAddress addr;
    char buf[1024];
    while(udpsocket->hasPendingDatagrams())
    {
        memset(buf, 0, sizeof(buf));
        udpsocket->readDatagram(buf, sizeof(buf), &addr);
        QMessageBox::information(this, tr("消息"), tr("来自") + addr.toString() + tr("的消息:\n") + buf);
    }
}


