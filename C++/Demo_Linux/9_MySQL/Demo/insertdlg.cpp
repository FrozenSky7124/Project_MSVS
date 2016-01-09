#include "insertdlg.h"
#include <QGridLayout>
#include <QMessageBox>

insertDlg::insertDlg()
{
    isokBtn = false;

    label1 = new QLabel(tr("姓名"), this);
    label2 = new QLabel(tr("性别"), this);
    label3 = new QLabel(tr("年龄"), this);
    label4 = new QLabel(tr("种族"), this);
    label5 = new QLabel(tr("门派"), this);
    leditName = new QLineEdit;
    leditSex = new QLineEdit;
    leditAge = new QLineEdit;
    leditRace = new QLineEdit;
    leditSchool = new QLineEdit;
    okBtn = new QPushButton(tr("插入"), this);
    cancelBtn = new QPushButton(tr("取消"), this);

    QGridLayout *layout1 = new QGridLayout(this);
    layout1->addWidget(label1, 0, 0);
    layout1->addWidget(label2, 1, 0);
    layout1->addWidget(label3, 2, 0);
    layout1->addWidget(label4, 3, 0);
    layout1->addWidget(label5, 4, 0);
    layout1->addWidget(leditName, 0, 1);
    layout1->addWidget(leditSex, 1, 1);
    layout1->addWidget(leditAge, 2, 1);
    layout1->addWidget(leditRace, 3, 1);
    layout1->addWidget(leditSchool, 4, 1);
    layout1->addWidget(okBtn, 5, 0);
    layout1->addWidget(cancelBtn, 5, 1);

    //美工代码区
    this->setWindowTitle(tr("插入数据"));
    this->setWindowIcon(QIcon("img_Icon.jpg"));
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("img_Dlg.jpg")));
    this->setPalette(palette);
    layout1->setMargin(15);
    layout1->setSpacing(10);
    layout1->setSizeConstraint(QLayout::SetFixedSize);

    connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(on_okBtn()));
    connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(on_cancelBtn()));
}

void insertDlg::on_okBtn()
{
    if(     (leditName->text() == "") ||
            (leditSex->text() == "") ||
            (leditAge->text() == "") ||
            (leditRace->text() == "") ||
            (leditSchool->text() == "")
      )
    {
        QMessageBox::information(this, "Error", "请输入正确的数据！");
    }
    else
    {
        isokBtn = true;
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "INSERT INTO pal_people (name, sex, age, race, school) VALUES (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\')",
                leditName->text().toStdString().data(), leditSex->text().toStdString().data(), leditAge->text().toStdString().data(),
                leditRace->text().toStdString().data(), leditSchool->text().toStdString().data());
        SQL = buf;
        this->close();
    }

}

void insertDlg::on_cancelBtn()
{
    this->close();
}

