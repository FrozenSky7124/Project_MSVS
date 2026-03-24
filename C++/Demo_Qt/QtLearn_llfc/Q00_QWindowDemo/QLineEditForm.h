#pragma once

#include <QDialog>
#include "ui_QLineEditForm.h"

class QLineEditForm : public QDialog
{
	Q_OBJECT

public:
	QLineEditForm(QDialog *parent = Q_NULLPTR);
	~QLineEditForm();

private:
	Ui::QLineEditForm ui;
};
