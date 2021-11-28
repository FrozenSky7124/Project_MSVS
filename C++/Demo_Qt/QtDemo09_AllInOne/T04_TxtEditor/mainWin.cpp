#include "mainWin.h"
#include "ui_mainWin.h"

mainWin::mainWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWin)
{
    ui->setupUi(this);
    initUI();
    initSIGNAL();
    setCentralWidget(ui->textEdit);
}

mainWin::~mainWin()
{
    delete ui;
}

void mainWin::initUI()
{
    label1 = new QLabel("Current File:");
    ui->statusbar->addWidget(label1);

    progressBar = new QProgressBar;
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(ui->textEdit->font().pointSize());
    ui->statusbar->addWidget(progressBar);

    spinBox_FontSize = new QSpinBox;
    spinBox_FontSize->setMinimum(0);
    spinBox_FontSize->setMaximum(100);
    spinBox_FontSize->setValue(ui->textEdit->font().pointSize());
    ui->toolBar->addWidget(new QLabel("FontSize:"));
    ui->toolBar->addWidget(spinBox_FontSize);

    ui->toolBar->addSeparator();

    fontComb_FontFamily = new QFontComboBox;
    ui->toolBar->addWidget(new QLabel("FontFamily:"));
    ui->toolBar->addWidget(fontComb_FontFamily);
}

void mainWin::initSIGNAL()
{
    connect(spinBox_FontSize, SIGNAL(valueChanged(int)),
            this, SLOT(customSLOT_spinBoxFontSize_valueChanged(int)));
    connect(fontComb_FontFamily, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(customSLOT_fontComb_FontFamily_indexChanged(QString)));
}

void mainWin::on_actionBold_triggered(bool checked)
{
    QTextCharFormat fmt;
    if (checked)
        fmt.setFontWeight(QFont::Bold);
    else
        fmt.setFontWeight(QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void mainWin::customSLOT_spinBoxFontSize_valueChanged(int fontSize)
{
    QTextCharFormat fmt;
    fmt = ui->textEdit->currentCharFormat();
    fmt.setFontPointSize(qreal(fontSize));
    ui->textEdit->mergeCurrentCharFormat(fmt);
    progressBar->setValue(fontSize);
}

void mainWin::customSLOT_fontComb_FontFamily_indexChanged(QString fontFamily)
{
    QTextCharFormat fmt;
    fmt = ui->textEdit->currentCharFormat();
    fmt.setFontFamily(fontFamily);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}
