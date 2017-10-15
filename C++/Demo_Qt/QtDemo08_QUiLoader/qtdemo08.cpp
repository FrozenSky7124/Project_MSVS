#include "qtdemo08.h"
#include "ui_qtdemo08.h"

QtDemo08::QtDemo08(QWidget *parent) : QDialog(parent), ui(new Ui::QtDemo08)
{
    ui->setupUi(this);
}

QtDemo08::~QtDemo08()
{
    delete ui;
}
