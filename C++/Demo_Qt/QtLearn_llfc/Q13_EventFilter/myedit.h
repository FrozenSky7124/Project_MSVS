#ifndef MYEDIT_H
#define MYEDIT_H

#include <QDebug>
#include <QLineEdit>

class MyEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyEdit(QWidget* parent = nullptr);
protected:
    virtual void keyPressEvent(QKeyEvent *event);
    bool event(QEvent * pEvent);
};

#endif // MYEDIT_H
