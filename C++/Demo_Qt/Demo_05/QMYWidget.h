#ifndef QMYWIDGET_H
#define QMYWIDGET_H

#include <QWidget>
#include "QPerson.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QMYWidget; }
QT_END_NAMESPACE

class QMYWidget : public QWidget
{
    Q_OBJECT

private:
    QPerson *KeQing;
    QPerson *ShenHe;

public:
    QMYWidget(QWidget *parent = nullptr);
    ~QMYWidget();

private:
    Ui::QMYWidget *ui;

private slots:
    void onMY_ageChanged(int value);
    void onMY_spinBox_valueChanged(int arg1);
    void onMY_btnAge_clicked();

    void on_btnMetaInfo_clicked();
};
#endif // QMYWIDGET_H
