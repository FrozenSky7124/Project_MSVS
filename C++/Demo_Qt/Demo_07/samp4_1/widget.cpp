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

