#include "T02_Dialog.h"
#include "ui_T02_Dialog.h"

T02_Dialog::T02_Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::T02_Dialog)
{
    ui->setupUi(this);
    QObject::connect(ui->radioButton_Blue,  SIGNAL(clicked()), this, SLOT(on_radioButtonClick()));
    QObject::connect(ui->radioButton_Green, SIGNAL(clicked()), this, SLOT(on_radioButtonClick()));
    QObject::connect(ui->radioButton_Red,   SIGNAL(clicked()), this, SLOT(on_radioButtonClick()));
}

T02_Dialog::~T02_Dialog()
{
    delete ui;
}

void T02_Dialog::on_checkBox_Bold_clicked(bool checked)
{
    QFont font = ui->plainTextEdit->font();
    font.setBold(checked);
    ui->plainTextEdit->setFont(font);
}

void T02_Dialog::on_checkBox_Italic_clicked(bool checked)
{
    QFont font = ui->plainTextEdit->font();
    font.setItalic(checked);
    ui->plainTextEdit->setFont(font);
}


void T02_Dialog::on_checkBox_Underline_clicked(bool checked)
{
    QFont font = ui->plainTextEdit->font();
    font.setUnderline(checked);
    ui->plainTextEdit->setFont(font);
}

void T02_Dialog::on_radioButtonClick()
{
    QPalette pale = ui->plainTextEdit->palette();
    pale.setColor(QPalette::Text, Qt::black);
    if (ui->radioButton_Blue->isChecked())
        pale.setColor(QPalette::Text, Qt::blue);
    else if (ui->radioButton_Green->isChecked())
        pale.setColor(QPalette::Text, Qt::green);
    else if (ui->radioButton_Red->isChecked())
        pale.setColor(QPalette::Text, Qt::red);
    ui->plainTextEdit->setPalette(pale);
}
