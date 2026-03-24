#include "QLineEditForm.h"
#include <QRegExpValidator>

QLineEditForm::QLineEditForm(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.editIP->setInputMask("000.000.000.000");
	ui.editMAC->setInputMask("HH:HH:HH:HH:HH:HH;_");
	ui.editPwd->setEchoMode(QLineEdit::Password);
	QRegExp regx("[a-zA-Z0-9_-]+@[a-zA-Z0-9]+\.[a-zA-Z0-9]");
	QValidator * vali = new QRegExpValidator(regx, ui.editEmail);
	ui.editEmail->setValidator(vali);
}

QLineEditForm::~QLineEditForm()
{
	
}
