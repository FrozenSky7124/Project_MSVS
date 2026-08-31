#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    pMyEdit = new MyEdit(this);
    pMyEdit->setFixedSize(200, 50);
    pMyEdit->move((this->width() - pMyEdit->width()) / 2, (this->height() - pMyEdit->height()) / 2);
    pMyEdit->installEventFilter(this);
    ui->spinBox->installEventFilter(this);
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == pMyEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            qDebug() << "[INF] Widget > eventFilter > pMyEdit";
        }
    }
    if (watched == ui->spinBox)
    {
        if (event->type() == QEvent::Wheel)
        {
            qDebug() << "[INF] Widget > eventFilter > spinBox";
        }
    }
    //return QWidget::eventFilter(watched, event);
    return false;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "[INF] Widget > keyPressEvent";
}
