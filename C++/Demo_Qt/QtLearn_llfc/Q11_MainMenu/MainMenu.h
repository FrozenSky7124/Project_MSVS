#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainMenu;
}
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu() override;

private slots:
    void on_actionNew_N_triggered();

    void on_actionDock_D_triggered();

private:
    Ui::MainMenu *ui;
};
#endif // MAINMENU_H
