#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QTimer>

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

	void on_btnProgress_clicked();

	void on_updateProgressDialog();

	void on_cancelProgressDialog();

private:
    Ui::MainWindow *ui;
	QProgressDialog *m_pProgressDlg;
	QTimer *m_pTimer;
	int m_iCount;
};

#endif // MAINWINDOW_H
