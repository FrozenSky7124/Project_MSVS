#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btnCalc_clicked()
{
    QString str;
    str = ui->lineEditNum->text();
    int num = str.toInt();
    str = ui->lineEditPrice->text();
    double price = str.toDouble();
    double total = num * price;
    str = str.sprintf("%.2f", total);
    ui->lineEditAll->setText(str);
}


void Widget::on_btnConvDec_clicked()
{
    QString str;
    str = ui->editDec->text();
    int iDec = str.toInt();
    str = str.setNum(iDec, 2);
    str = str.toUpper();
    ui->editBin->setText(str);
    str = str.setNum(iDec, 16);
    str = str.toUpper();
    ui->editHex->setText(str);
}


void Widget::on_btnConvBin_clicked()
{
    QString str;
    str = ui->editBin->text();
    bool ok;
    int iBin = str.toInt(&ok, 2);
    str = QString::number(iBin, 10);
    ui->editDec->setText(str);
    str = QString::number(iBin, 16);
    str = str.toUpper();
    ui->editHex->setText(str);
}

