#include "MyAction.h"
#include <QLabel>
#include <QSplitter>

MyAction::MyAction(QObject * parent) : QWidgetAction(parent)
{
    pLineEdit = new QLineEdit();
    QObject::connect(pLineEdit, &QLineEdit::returnPressed, this, &MyAction::sendText);
}

QWidget *MyAction::createWidget(QWidget *parent)
{
    if ((!(parent->inherits("QMenu"))) && (!(parent->inherits("QToolBar"))))
    {
        return 0;
    }
    QSplitter * pSplitter = new QSplitter(parent);
    QLabel * pLabel = new QLabel();
    pLabel->setText("TEXT:");
    pSplitter->addWidget(pLabel);
    pSplitter->addWidget(pLineEdit);
    return pSplitter;
}

void MyAction::sendText()
{
    emit getText(pLineEdit->text());
    pLineEdit->clear();
}