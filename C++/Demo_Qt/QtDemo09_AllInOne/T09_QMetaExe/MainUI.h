#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>
#include "QPlayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainUI; }
QT_END_NAMESPACE

class MainUI : public QWidget
{
    Q_OBJECT

public:
    MainUI(QWidget *parent = nullptr);
    ~MainUI();

private:
    Ui::MainUI *ui;

private:
    QPlayer *playerA;
    QPlayer *playerB;

    void on_spinBox_valueChanged(unsigned int argv1);
    void on_playerLevelChanged(unsigned int level);

private slots:
    void on_btnAUp_clicked();
    void on_btnBUp_clicked();
    void on_btnMInfo_clicked();
    void on_spinBoxA_valueChanged(int value);
};
#endif // MAINUI_H
