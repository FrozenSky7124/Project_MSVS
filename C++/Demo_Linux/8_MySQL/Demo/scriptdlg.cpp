#include "scriptdlg.h"
#include <QGridLayout>
#include <QHBoxLayout>

scriptDlg::scriptDlg()
{
    islogin = false;
    label0 = new QLabel;
    label0->setText(tr("Enter SQL:"));
    textEditSQL = new QTextEdit;
    okBtn = new QPushButton;
    cancelBtn = new QPushButton;
    okBtn->setText(tr("GO！"));
    cancelBtn->setText(tr("Cancel"));
    textEditSQL->setText("SELECT * FROM pal_people");

    QGridLayout *layout1 = new QGridLayout(this);
    layout1->addWidget(label0, 0, 0);
    layout1->addWidget(textEditSQL, 0, 1);

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(okBtn);
    layout2->addWidget(cancelBtn);

    layout1->addLayout(layout2, 1, 1);

    //美工代码区
    this->setWindowTitle(tr("SQL Script"));
    this->setWindowIcon(QIcon("img_Icon.jpg"));
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("img_Dlg.jpg")));
    this->setPalette(palette);
    layout1->setSizeConstraint(QLayout::SetFixedSize);

    connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(okBtnOnclick()));
    connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(cancelBtnOnclick()));
}

void scriptDlg::okBtnOnclick()
{
    islogin = true;
    SQL = textEditSQL->toPlainText();//获得SQL语句
    close();
}

void scriptDlg::cancelBtnOnclick()
{
    close();
}

