#include "register.h"
#include "ui_ui_register.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

Register::Register(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Register"));
    this->setMinimumSize(400, 500);
    this->setMaximumSize(400, 500);
    
    auto layoutEmail = new QHBoxLayout();
    auto labelEmail = new QLabel();
    labelEmail->setText(tr("Email:"));
    auto lineEditEmail = new QLineEdit();
    layoutEmail->addWidget(labelEmail);
    layoutEmail->addWidget(lineEditEmail);
    
    auto layoutVMain = new QVBoxLayout(); // Main Layout
    auto spacerV1 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layoutVMain->addItem(spacerV1);
    layoutVMain->addLayout(layoutEmail);

    this->setLayout(layoutVMain);
}

Register::~Register()
{
    delete ui;
}
