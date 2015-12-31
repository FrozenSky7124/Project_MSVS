#ifndef DRAWER_H
#define DRAWER_H

#include <QWidget>
#include <QToolBox>
#include <QToolButton>
#include "widget.h"

class drawer : public QToolBox
{
    Q_OBJECT
public:
    explicit drawer(QWidget *parent = 0);

private:
    QToolButton *toolBtn1;
    QToolButton *toolBtn2;
    QToolButton *toolBtn3;
    QToolButton *toolBtn4;
    Widget *w;

private slots:
    void showWidget();
};

#endif // DRAWER_H
