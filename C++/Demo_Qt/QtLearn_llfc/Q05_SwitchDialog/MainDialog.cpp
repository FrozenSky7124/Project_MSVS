#include "MainDialog.h"
#include "ui_MainDialog.h"
#include "ChildDialog.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_btnEnter_clicked()
{
    this->close();
    ChildDialog childDlg;
    if (childDlg.exec() == QDialog::Accepted)
    {
        this->show();
    }
    else
    {
        return;
    }
}
