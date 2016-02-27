#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    bool dbonline;
    QLineEdit *SearchLine;
    QPushButton *HelpBtn;
    QTextEdit *ResultTextEdit;
    bool linkToDatabase();
    bool isRepeatQA(QStringList &list, QString &value);

private slots:
    void onSearchChange();
    void onHelpBtn();

protected:
    bool nativeEvent(const QByteArray & eventType, void * message, long * result);
};

#endif // MAINWIN_H
