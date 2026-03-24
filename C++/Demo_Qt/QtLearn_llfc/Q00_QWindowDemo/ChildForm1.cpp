#include "ChildForm1.h"
#include "ui_ChildForm1.h"
#include <QDebug>

ChildForm1::ChildForm1(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::ChildForm1();
	ui->setupUi(this);

	pParent = parent;

	//Qt5 connect
	QObject::connect(ui->BtnToMain, &QPushButton::clicked, this, &ChildForm1::BackToMainWindow);
}

ChildForm1::~ChildForm1()
{
	delete ui;
	qDebug() << "[INF] ChildForm1 > ~ChildForm1()";
}

void ChildForm1::BackToMainWindow()
{
	this->hide();
	//pParent->show();          // use member var to obtain parent
	//parentWidget()->show();   // use function to obtain parent
	emit SIG_showMainForm();    // use signal 
}
