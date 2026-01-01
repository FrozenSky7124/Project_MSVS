#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnColor_clicked();

    void on_btnFile_clicked();

    void on_btnInputText_clicked();

    void on_btnInputInt_clicked();

    void on_btnInputDouble_clicked();

    void on_btnItem_clicked();

    void on_btnMessage_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
