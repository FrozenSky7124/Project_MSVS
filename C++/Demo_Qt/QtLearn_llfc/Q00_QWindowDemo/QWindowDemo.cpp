#include "QWindowDemo.h"
#include <QDialog>
#include <QDebug>

QWindowDemo::QWindowDemo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	pChildForm1 = new ChildForm1(this);
	// Qt4 connect
	//QObject::connect(ui.BtnShowChildDlg, SIGNAL(clicked()), this, SLOT(showChildDialog()));
	// Qt5 connect
	QObject::connect(ui.BtnShowChildDlg, &QPushButton::clicked, this, &QWindowDemo::showChildDialog);
	QObject::connect(pChildForm1, &ChildForm1::SIG_showMainForm, this, &QWindowDemo::showMainForm);
	//QObject::connect(pChildForm1, SIGNAL(SIG_showMainForm()), this, SLOT(showMainForm()));
	QObject::connect(ui.BtnQLineEditDemo, &QPushButton::clicked, this, &QWindowDemo::showQLineEditForm);

	qDebug() << "[INF] QWindowDemo > QWindowDemo()";
}

QWindowDemo::~QWindowDemo()
{
	if (pChildForm1)
	{
		delete pChildForm1;
		pChildForm1 = nullptr;
	}
	qDebug() << "[INF] QWindowDemo > ~QWindowDemo()";
}

void QWindowDemo::showChildDialog()
{
	this->hide();
	pChildForm1->show();
	return;
}

void QWindowDemo::showMainForm()
{
	this->show();
}

void QWindowDemo::showQLineEditForm()
{
	QLineEditForm form;
	form.setWindowModality(Qt::WindowModality::ApplicationModal);
	form.exec();
}