#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QWindowDemo.h"
#include "ChildForm1.h"

class QWindowDemo : public QMainWindow
{
    Q_OBJECT

public:
    QWindowDemo(QWidget *parent = Q_NULLPTR);
	~QWindowDemo();

private:
    Ui::QWindowDemoClass ui;
	ChildForm1* pChildForm1;

public slots:
	void showChildDialog();
	void showMainForm();
};
