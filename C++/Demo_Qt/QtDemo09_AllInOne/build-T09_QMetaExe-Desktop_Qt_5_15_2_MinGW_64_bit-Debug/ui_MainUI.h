/********************************************************************************
** Form generated from reading UI file 'MainUI.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainUI
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *spinBoxA;
    QPushButton *btnAUp;
    QPushButton *btnMInfo;
    QLabel *label_2;
    QSpinBox *spinBoxB;
    QPushButton *btnBUp;
    QPushButton *btnClear;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *MainUI)
    {
        if (MainUI->objectName().isEmpty())
            MainUI->setObjectName(QString::fromUtf8("MainUI"));
        MainUI->resize(631, 375);
        verticalLayout = new QVBoxLayout(MainUI);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(MainUI);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        spinBoxA = new QSpinBox(MainUI);
        spinBoxA->setObjectName(QString::fromUtf8("spinBoxA"));

        gridLayout->addWidget(spinBoxA, 0, 1, 1, 1);

        btnAUp = new QPushButton(MainUI);
        btnAUp->setObjectName(QString::fromUtf8("btnAUp"));

        gridLayout->addWidget(btnAUp, 0, 2, 1, 1);

        btnMInfo = new QPushButton(MainUI);
        btnMInfo->setObjectName(QString::fromUtf8("btnMInfo"));

        gridLayout->addWidget(btnMInfo, 0, 3, 1, 1);

        label_2 = new QLabel(MainUI);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        spinBoxB = new QSpinBox(MainUI);
        spinBoxB->setObjectName(QString::fromUtf8("spinBoxB"));

        gridLayout->addWidget(spinBoxB, 1, 1, 1, 1);

        btnBUp = new QPushButton(MainUI);
        btnBUp->setObjectName(QString::fromUtf8("btnBUp"));

        gridLayout->addWidget(btnBUp, 1, 2, 1, 1);

        btnClear = new QPushButton(MainUI);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));

        gridLayout->addWidget(btnClear, 1, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        plainTextEdit = new QPlainTextEdit(MainUI);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        verticalLayout->addWidget(plainTextEdit);


        retranslateUi(MainUI);
        QObject::connect(btnClear, SIGNAL(clicked()), plainTextEdit, SLOT(clear()));

        QMetaObject::connectSlotsByName(MainUI);
    } // setupUi

    void retranslateUi(QWidget *MainUI)
    {
        MainUI->setWindowTitle(QCoreApplication::translate("MainUI", "MainUI", nullptr));
        label->setText(QCoreApplication::translate("MainUI", "PlayerA Level", nullptr));
        btnAUp->setText(QCoreApplication::translate("MainUI", "PlayerA LevelUp", nullptr));
        btnMInfo->setText(QCoreApplication::translate("MainUI", "Class MetaObject INFO", nullptr));
        label_2->setText(QCoreApplication::translate("MainUI", "PlayerB Level", nullptr));
        btnBUp->setText(QCoreApplication::translate("MainUI", "PlayerB LevelUp", nullptr));
        btnClear->setText(QCoreApplication::translate("MainUI", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainUI: public Ui_MainUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
