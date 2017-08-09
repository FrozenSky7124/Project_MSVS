#include <QLayout>
#include "ExtensionDialog.h"
#include "ui_ExtensionDialog.h"

ExtensionDialog::ExtensionDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ExtensionDialog)
{
    ui->setupUi(this);
    ui->groupBox_2->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

ExtensionDialog::~ExtensionDialog()
{
    delete ui;
}

void ExtensionDialog::on_pushButton_clicked()
{
    this->close();
}

void ExtensionDialog::on_pushButton_2_clicked()
{
    if (ui->groupBox_2->isVisible())
    {
        ui->groupBox_2->hide();
        ui->pushButton_2->setText("Show   >>>");
    }
    else
    {
        ui->groupBox_2->show();
        ui->pushButton_2->setText("Hide   <<<");
    }
}
