#include "MainDialog.h"

void MainDialog::initUI()
{
    chkBox_Bold = new QCheckBox("粗体");
    chkBox_Italic = new QCheckBox("斜体");
    chkBox_Underline = new QCheckBox("下划线");

    radBtn_Blue = new QRadioButton("Blue");
    radBtn_Yellow = new QRadioButton("Yellow");
    radBtn_Red = new QRadioButton("Red");

    plainTxt= new QPlainTextEdit("剑光如我，斩尽芜杂！");
    QFont txtFont = plainTxt->font();
    txtFont.setPointSize(17);
    txtFont.setFamily("楷体");
    plainTxt->setFont(txtFont);

    btnOK = new QPushButton("OK");
    btnExit = new QPushButton("EXIT");

    HLayout1 = new QHBoxLayout;
    HLayout1->addWidget(chkBox_Bold);
    HLayout1->addWidget(chkBox_Italic);
    HLayout1->addWidget(chkBox_Underline);

    HLayout2 = new QHBoxLayout;
    HLayout2->addWidget(radBtn_Blue);
    HLayout2->addWidget(radBtn_Yellow);
    HLayout2->addWidget(radBtn_Red);

    HLayout3 = new QHBoxLayout;
    HLayout3->addStretch();
    HLayout3->addWidget(btnOK);
    HLayout3->addStretch();
    HLayout3->addWidget(btnExit);

    VLayout1 = new QVBoxLayout;
    VLayout1->addLayout(HLayout1);
    VLayout1->addLayout(HLayout2);
    VLayout1->addWidget(plainTxt);
    VLayout1->addLayout(HLayout3);

    setLayout(VLayout1);
}

void MainDialog::initSignal()
{
    connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(chkBox_Bold, SIGNAL(clicked(bool)), this, SLOT(on_chkBox_Bold_clicked(bool)));
    connect(chkBox_Italic, SIGNAL(clicked(bool)), this, SLOT(on_chkBox_Italic_clicked(bool)));
    connect(chkBox_Underline, SIGNAL(clicked(bool)), this, SLOT(on_chkBox_Underline_clicked(bool)));
    connect(radBtn_Blue, SIGNAL(clicked()), this, SLOT(on_radioButtonClick()));
    connect(radBtn_Yellow, SIGNAL(clicked()), this, SLOT(on_radioButtonClick()));
    connect(radBtn_Red, SIGNAL(clicked()), this, SLOT(on_radioButtonClick()));
}

void MainDialog::on_chkBox_Bold_clicked(bool check)
{
    QFont txtFont = plainTxt->font();
    txtFont.setBold(check);
    plainTxt->setFont(txtFont);
}

void MainDialog::on_chkBox_Italic_clicked(bool check)
{
    QFont txtFont = plainTxt->font();
    txtFont.setItalic(check);
    plainTxt->setFont(txtFont);
}

void MainDialog::on_chkBox_Underline_clicked(bool check)
{
    QFont txtFont = plainTxt->font();
    txtFont.setUnderline(check);
    plainTxt->setFont(txtFont);
}

void MainDialog::on_radioButtonClick()
{
    QPalette pale = plainTxt->palette();
    pale.setColor(QPalette::Text, Qt::black);
    if (radBtn_Blue->isChecked())
        pale.setColor(QPalette::Text, Qt::blue);
    else if (radBtn_Yellow->isChecked())
        pale.setColor(QPalette::Text, Qt::yellow);
    else if (radBtn_Red->isChecked())
        pale.setColor(QPalette::Text, Qt::red);
    plainTxt->setPalette(pale);
}

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{
    initUI();
    initSignal();
}

MainDialog::~MainDialog()
{
}

