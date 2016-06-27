#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QLayout>
#include <QEvent>
#include <QSystemTrayIcon>

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
    //托盘区图标
    QSystemTrayIcon *trayicon;
    QMenu *trayiconMenu;
    bool linkToDatabase();
    bool isRepeatQA(QStringList &list, QString &value);

private slots:
    void onSearchChange();
    void onHelpBtn();
    //托盘区图标响应槽函数
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);

protected:
    bool nativeEvent(const QByteArray & eventType, void * message, long * result);
    void changeEvent(QEvent *event);
};

#endif // MAINWIN_H
