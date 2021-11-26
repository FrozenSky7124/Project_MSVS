#ifndef T01_WIDGET_H
#define T01_WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class T01_Widget; }
QT_END_NAMESPACE

class T01_Widget : public QWidget
{
    Q_OBJECT

public:
    T01_Widget(QWidget *parent = nullptr);
    ~T01_Widget();

private:
    Ui::T01_Widget *ui;
};
#endif // T01_WIDGET_H
