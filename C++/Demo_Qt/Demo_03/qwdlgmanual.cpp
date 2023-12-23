#include "qwdlgmanual.h"
#include <QLayout>

void QWDlgManual::initUI()
{
    chkBoxUnder = new QCheckBox(tr("Underline"));
    chkBoxItalic = new QCheckBox(tr("Italic"));
    chkBoxBold = new QCheckBox(tr("Bold"));
    QHBoxLayout* HLay1 = new QHBoxLayout();
    HLay1->addWidget(chkBoxUnder);
    HLay1->addWidget(chkBoxItalic);
    HLay1->addWidget(chkBoxBold);

    btnOK = new QPushButton(tr("OK"));
    btnCancel = new QPushButton(tr("Cancel"));
    btnClose = new QPushButton(tr("Close"));
    QHBoxLayout* HLay3 = new QHBoxLayout();
    HLay3->addWidget(btnOK);
    HLay3->addWidget(btnCancel);
    HLay3->addStretch();
    HLay3->addWidget(btnClose);

    txtEdit = new QPlainTextEdit();
    txtEdit->setPlainText("Hello World!\nQt Demo 03.");
    QFont font = txtEdit->font();
    font.setPointSize(20);
    txtEdit->setFont(font);

    QVBoxLayout* VLay = new QVBoxLayout();
    VLay->addLayout(HLay1);
    VLay->addWidget(txtEdit);
    VLay->addLayout(HLay3);
    setLayout(VLay);
}

void QWDlgManual::initSignalSlots()
{
    QObject::connect(chkBoxUnder, SIGNAL(clicked()), this, SLOT(on_click_chkBox()));
    QObject::connect(chkBoxItalic, SIGNAL(clicked()), this, SLOT(on_click_chkBox()));
    QObject::connect(chkBoxBold, SIGNAL(clicked()), this, SLOT(on_click_chkBox()));
}

void QWDlgManual::on_click_chkBox()
{
    QFont font = txtEdit->font();
    if (chkBoxUnder->isChecked()) font.setUnderline(true);
    else font.setUnderline(false);
    if (chkBoxItalic->isChecked()) font.setItalic(true);
    else font.setItalic(false);
    if (chkBoxBold->isChecked()) font.setBold(true);
    else font.setBold(false);
    txtEdit->setFont(font);
}

QWDlgManual::QWDlgManual(QWidget *parent)
    : QDialog(parent)
{
    this->initUI();
    this->initSignalSlots();
    setWindowTitle(tr("Demo_03 Manual UI"));
}

QWDlgManual::~QWDlgManual()
{
}

