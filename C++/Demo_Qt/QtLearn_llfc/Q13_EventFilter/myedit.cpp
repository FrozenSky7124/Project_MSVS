#include "myedit.h"
#include <QKeyEvent>

MyEdit::MyEdit(QWidget *parent):QLineEdit(parent)
{

}

void MyEdit::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "[INF] MyEdit > keyPressEvent";
    QLineEdit::keyPressEvent(event);
    event->ignore();
}

bool MyEdit::event(QEvent *pEvent)
{
    if (pEvent->type() == QEvent::KeyPress)
    {
        qDebug() << "[INF] MyEdit > event";
    }
    return QLineEdit::event(pEvent);
}
