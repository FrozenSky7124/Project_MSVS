#include "T01_Widget.h"
#include "ui_T01_Widget.h"

T01_Widget::T01_Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::T01_Widget)
{
    ui->setupUi(this);
}

T01_Widget::~T01_Widget()
{
    delete ui;
}

