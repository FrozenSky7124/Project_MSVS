#pragma once

#include <QDialog>

namespace Ui { class ChildForm1; };

class ChildForm1 : public QDialog
{
	Q_OBJECT

signals:
	void SIG_showMainForm();

public:
	ChildForm1(QWidget *parent = Q_NULLPTR);
	~ChildForm1();

private:
	Ui::ChildForm1 *ui;
	QWidget* pParent;

public slots:
	void BackToMainWindow();
};
