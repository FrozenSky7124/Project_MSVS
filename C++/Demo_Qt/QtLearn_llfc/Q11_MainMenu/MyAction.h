#ifndef MYACTION_H
#define MYACTION_H

#include <QWidgetAction>
#include <QLineEdit>

class MyAction : public QWidgetAction
{

    Q_OBJECT

signals:
    void getText(const QString& str);

public:
    MyAction(QObject * parent = nullptr);

protected:
    virtual QWidget * createWidget(QWidget * parent);

private:
    QLineEdit * pLineEdit;

private slots:
    void sendText();
};

#endif // MYACTION_H
