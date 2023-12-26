#include "QMYWidget.h"
#include "ui_QMYWidget.h"
#include <QMetaProperty>

QMYWidget::QMYWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QMYWidget)
{
    ui->setupUi(this);

    KeQing = new QPerson("刻晴");
    KeQing->setProperty("score", 95);
    KeQing->setProperty("age", 24);
    KeQing->setProperty("sex", "Girl"); // Dynamic Property
    connect(KeQing, &QPerson::ageChanged, this, &QMYWidget::onMY_ageChanged);

    ShenHe = new QPerson("申鹤");
    ShenHe->setProperty("score", 90);
    ShenHe->setProperty("age", 25);
    ShenHe->setProperty("sex", "Girl"); // Dynamic Property
    connect(ShenHe, &QPerson::ageChanged, this, &QMYWidget::onMY_ageChanged);

    ui->spinBox1->setProperty("isKeQing", true); // Dynamic Property
    ui->spinBox2->setProperty("isShenHe", true); // Dynamic Property
    ui->spinBox1->setValue(KeQing->property("age").toInt());
    ui->spinBox2->setValue(ShenHe->property("age").toInt());

    connect(ui->spinBox1, SIGNAL(valueChanged(int)), this, SLOT(onMY_spinBox_valueChanged(int)));
    connect(ui->spinBox2, SIGNAL(valueChanged(int)), this, SLOT(onMY_spinBox_valueChanged(int)));

    ui->btnAgeAdd1->setProperty("isKeQing", true); // Dynamic Property
    ui->btnAgeAdd2->setProperty("isShenHe", true); // Dynamic Property
    connect(ui->btnAgeAdd1, SIGNAL(clicked()), this, SLOT(onMY_btnAge_clicked()));
    connect(ui->btnAgeAdd2, SIGNAL(clicked()), this, SLOT(onMY_btnAge_clicked()));
}

QMYWidget::~QMYWidget()
{
    delete ui;
}

void QMYWidget::onMY_ageChanged(int value)
{
    Q_UNUSED(value);
    QPerson* aPerson = qobject_cast<QPerson*>(sender());
    QString aName = aPerson->property("name").toString();
    QString aSex = aPerson->property("sex").toString();
    int aAge = aPerson->age();
    //int aAge = aPerson->property("age").toInt();
    ui->plainTextEdit->appendPlainText(aName + ", " + aSex + ", " + QString::asprintf("Age = %d", aAge));
}

void QMYWidget::onMY_spinBox_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(sender());
    if (spinBox->property("isKeQing").toBool() == true)
        KeQing->setAge(spinBox->value());
    else
        ShenHe->setAge(spinBox->value());
}

void QMYWidget::onMY_btnAge_clicked()
{
    QPushButton* pushBtn = qobject_cast<QPushButton*>(sender());
    if (pushBtn->property("isKeQing").toBool() == true)
    {
        KeQing->incAge();
        ui->spinBox1->setValue(KeQing->age());
    }
    else
    {
        ShenHe->incAge();
        ui->spinBox2->setValue(ShenHe->age());
    }
}


void QMYWidget::on_btnMetaInfo_clicked()
{
    const QMetaObject* meta = KeQing->metaObject();
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText("=== MetaObject Info ===\n");
    ui->plainTextEdit->appendPlainText(
                QString("ClassName: %1\n").arg(meta->className()));
    ui->plainTextEdit->appendPlainText("Property");
    for (int i = meta->propertyOffset(); i < meta->propertyCount(); i++)
    {
        QMetaProperty prop = meta->property(i);
        const char* propName = prop.name();
        QString propValue = KeQing->property(propName).toString();
        ui->plainTextEdit->appendPlainText(
                    QString("PropName = %1, PropValue = %2").arg(propName, propValue));
    }

    ui->plainTextEdit->appendPlainText("");
    ui->plainTextEdit->appendPlainText("ClassInfo");
    for (int i = meta->classInfoOffset(); i < meta->classInfoCount(); i++)
    {
        QMetaClassInfo classinfo = meta->classInfo(i);
        ui->plainTextEdit->appendPlainText(
                    QString("Name = %1, Value = %2").arg(classinfo.name(), classinfo.value()));
    }
}

