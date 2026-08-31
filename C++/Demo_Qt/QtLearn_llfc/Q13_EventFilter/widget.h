#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myedit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::Widget *ui;
    MyEdit* pMyEdit;

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};
#endif // WIDGET_H
