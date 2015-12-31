#include "drawer.h"
#include "widget.h"
#include <QGroupBox>
#include <QVBoxLayout>

drawer::drawer(QWidget *parent) : QToolBox(parent)
{
    setWindowTitle(tr("QQ 2070"));

    toolBtn1 = new QToolButton;
    toolBtn1->setText(tr("AC1"));
    toolBtn1->setIcon(QPixmap("AC1.png"));
    toolBtn1->setIconSize(QPixmap("AC1.png").size());
    toolBtn1->setAutoRaise(true);//设置按钮的自动高亮属性
    toolBtn1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置文字相对位置

    toolBtn2 = new QToolButton;
    toolBtn2->setText(tr("AC2"));
    toolBtn2->setIcon(QPixmap("AC2.png"));
    toolBtn2->setIconSize(QPixmap("AC2.png").size());
    toolBtn2->setAutoRaise(true);//设置按钮的自动高亮属性
    toolBtn2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置文字相对位置

    toolBtn3 = new QToolButton;
    toolBtn3->setText(tr("AC3"));
    toolBtn3->setIcon(QPixmap("AC3.png"));
    toolBtn3->setIconSize(QPixmap("AC3.png").size());
    toolBtn3->setAutoRaise(true);//设置按钮的自动高亮属性
    toolBtn3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置文字相对位置

    toolBtn4 = new QToolButton;
    toolBtn4->setText(tr("AC4"));
    toolBtn4->setIcon(QPixmap("AC4.png"));
    toolBtn4->setIconSize(QPixmap("AC4.png").size());
    toolBtn4->setAutoRaise(true);//设置按钮的自动高亮属性
    toolBtn4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置文字相对位置

    QGroupBox *groupBox1 = new QGroupBox;
    QVBoxLayout *layout1 = new QVBoxLayout(groupBox1);
    layout1->setMargin(10);
    layout1->setAlignment(Qt::AlignHCenter);
    layout1->addWidget(toolBtn1);
    layout1->addWidget(toolBtn2);
    layout1->addStretch();//插入占位符

    QGroupBox *groupBox2 = new QGroupBox;
    QVBoxLayout *layout2 = new QVBoxLayout(groupBox2);
    layout2->setMargin(10);
    layout2->setAlignment(Qt::AlignHCenter);
    layout2->addWidget(toolBtn3);
    layout2->addWidget(toolBtn4);
    layout2->addStretch();//插入占位符

    this->addItem((QWidget *)groupBox1,tr("NICONICONI"));
    this->addItem((QWidget *)groupBox2,tr("POI~ POI~"));

    w = new Widget;
    w->resize(400,300);
    w->setWindowTitle(tr("NICONICONI"));


    connect(toolBtn1, SIGNAL(clicked()), this, SLOT(showWidget()));
    connect(toolBtn2, SIGNAL(clicked()), this, SLOT(showWidget()));
    connect(toolBtn3, SIGNAL(clicked()), this, SLOT(showWidget()));
    connect(toolBtn4, SIGNAL(clicked()), this, SLOT(showWidget()));
}

void drawer::showWidget()
{
    w->show();
}
