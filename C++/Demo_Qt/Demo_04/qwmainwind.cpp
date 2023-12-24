#include "qwmainwind.h"
#include "ui_qwmainwind.h"

QWMainWind::QWMainWind(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QWMainWind)
{
    ui->setupUi(this);
    initUI();
    initSS();
}

QWMainWind::~QWMainWind()
{
    delete ui;
}

void QWMainWind::initUI()
{
    fLabCurFile = new QLabel;
    fLabCurFile->setMinimumWidth(150);
    fLabCurFile->setText("CurFile:");
    ui->statusbar->addWidget(fLabCurFile);

    progressBar1 = new QProgressBar;
    progressBar1->setMaximumWidth(200);
    progressBar1->setMinimum(5);
    progressBar1->setMaximum(50);
    progressBar1->setValue(ui->txtEdit->font().pointSize());
    ui->statusbar->addWidget(progressBar1);

    spinFontSize = new QSpinBox;
    spinFontSize->setMinimumWidth(50);
    spinFontSize->setMinimum(5);
    spinFontSize->setMaximum(50);
    spinFontSize->setValue(ui->txtEdit->font().pointSize());
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(new QLabel("FontSize:"));
    ui->toolBar->addWidget(spinFontSize);

    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(new QLabel("FontFamily:"));
    comboFont = new QFontComboBox;
    comboFont->setMinimumWidth(150);
    ui->toolBar->addWidget(comboFont);

    setCentralWidget(ui->txtEdit);
}

void QWMainWind::initSS()
{
    connect(spinFontSize, SIGNAL(valueChanged(int)), this, SLOT(onMY_spinBoxFontSize_valueChanged(int)));
    connect(comboFont, SIGNAL(currentIndexChanged(QString)), this, SLOT(onMY_comboFont_currentIndexChanged(QString)));
}


void QWMainWind::on_actBold_triggered(bool checked)
{
    QTextCharFormat fmt;
    fmt = ui->txtEdit->currentCharFormat();
    if (checked)
        fmt.setFontWeight(QFont::Bold);
    else
        fmt.setFontWeight(QFont::Normal);
    ui->txtEdit->mergeCurrentCharFormat(fmt);
}


void QWMainWind::on_txtEdit_copyAvailable(bool b)
{
    ui->actCopy->setEnabled(b);
    //ui->actCut->setEnabled(b);
    //ui->actPaste->setEnabled(ui->txtEdit->canPaste());
}


void QWMainWind::on_txtEdit_selectionChanged()
{
    QTextCharFormat fmt;
    fmt = ui->txtEdit->currentCharFormat();
    ui->actBold->setChecked(fmt.font().bold());
    //ui->actItalic->setChecked(fmt.fontItalic());
    //ui->actUnder->setChecked(fmt.fontUnderline());
}


void QWMainWind::onMY_spinBoxFontSize_valueChanged(int aFontSize)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(aFontSize);
    ui->txtEdit->mergeCurrentCharFormat(fmt);
    progressBar1->setValue(aFontSize);
}

void QWMainWind::onMY_comboFont_currentIndexChanged(const QString &arg1)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(arg1);
    ui->txtEdit->mergeCurrentCharFormat(fmt);
}

