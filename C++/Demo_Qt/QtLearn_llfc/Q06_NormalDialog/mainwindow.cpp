#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	m_iCount = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnColor_clicked()
{
    QColorDialog colorDlg(Qt::blue, this);
    colorDlg.setOption(QColorDialog::ShowAlphaChannel);
    colorDlg.setWindowTitle(tr("颜色选择"));
    colorDlg.exec();
    QColor color = colorDlg.currentColor();

    //QColor color = QColorDialog::getColor(Qt::blue, this, tr("颜色选择"), QColorDialog::ShowAlphaChannel);
    qDebug() << "color is " << color;
}

void MainWindow::on_btnFile_clicked()
{
    QString path = QDir::currentPath();
    QString filter = tr("TXT Files(*.txt);;Picture Files(*.png *.jpg *.webp *.gif);;All Files(*.*)");
    QString fileName = QFileDialog::getOpenFileName(this, tr("文件选择"), path, filter);
    qDebug() << fileName;
}

void MainWindow::on_btnInputText_clicked()
{
    bool rst;
    auto text = QInputDialog::getText(this, tr("文本"), tr("文本"), QLineEdit::Normal, tr(""), &rst);
    if (rst) qDebug() << text;
}

void MainWindow::on_btnInputInt_clicked()
{
    bool rst;
    auto num = QInputDialog::getInt(this, tr("整型"), tr("Input Int"), 0, -5, 5, 1, &rst);
    if (rst) qDebug() << num;
}

void MainWindow::on_btnInputDouble_clicked()
{
    bool rst;
    auto num = QInputDialog::getDouble(this, tr("浮点型"), tr("Input Double"), 0.0, -5.5, 5.5, 1, &rst);
    if (rst) qDebug() << num;
}

void MainWindow::on_btnItem_clicked()
{
    QStringList list;
    list << tr("无名剑法") << tr("无名枪法") << tr("积矩九剑") << tr("九曲惊魂枪");
    bool rst;
    auto item = QInputDialog::getItem(this, tr("Skill"), tr("Select:"), list, 0, false, &rst);
    if (rst) qDebug() << item;
}

void MainWindow::on_btnMessage_clicked()
{
    QMessageBox::question(this,    tr("询问"), tr("YES or NO"),   QMessageBox::Yes, QMessageBox::No);
    QMessageBox::information(this, tr("通知"), tr("Information"), QMessageBox::Ok);
    QMessageBox::warning(this,     tr("警告"), tr("Warning"),     QMessageBox::Ok);
    QMessageBox::critical(this,    tr("关键"), tr("Critical"),    QMessageBox::Ok);
}

void MainWindow::on_btnProgress_clicked()
{
	m_pProgressDlg = new QProgressDialog(tr("正在复制"), tr("取消复制"), 0, 5000, this);
	m_pProgressDlg->setWindowTitle(tr("ProgressDialog"));
	m_pProgressDlg->setWindowModality(Qt::ApplicationModal);
	m_pProgressDlg->setMinimumSize(400, 80);
	m_pProgressDlg->resize(400, 80);
	m_pTimer = new QTimer(this);
	connect(m_pTimer, &QTimer::timeout, this, &MainWindow::on_updateProgressDialog);
	connect(m_pProgressDlg, &QProgressDialog::canceled, this, &MainWindow::on_cancelProgressDialog);
	m_pTimer->start(2);
}

void MainWindow::on_updateProgressDialog()
{
	m_iCount++;

	if (m_iCount > 5000)
	{
		m_pTimer->stop();
		delete m_pTimer;
		m_pTimer = nullptr;
		delete m_pProgressDlg;
		m_pProgressDlg = nullptr;
		m_iCount = 0;
		return;
	}
	
	m_pProgressDlg->setValue(m_iCount);
}

void MainWindow::on_cancelProgressDialog()
{
	m_pTimer->stop();
	delete m_pTimer;
	m_pTimer = nullptr;
	delete m_pProgressDlg;
	m_pProgressDlg = nullptr;
	m_iCount = 0;
	return;
}