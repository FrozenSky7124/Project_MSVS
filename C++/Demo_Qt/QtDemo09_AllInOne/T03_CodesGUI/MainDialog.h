#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QRadioButton>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MainDialog : public QDialog
{
    Q_OBJECT

private:
    QCheckBox *chkBox_Bold;
    QCheckBox *chkBox_Italic;
    QCheckBox *chkBox_Underline;

    QRadioButton *radBtn_Blue;
    QRadioButton *radBtn_Yellow;
    QRadioButton *radBtn_Red;

    QPlainTextEdit * plainTxt;

    QPushButton *btnOK;
    QPushButton *btnExit;

    QHBoxLayout *HLayout1;
    QHBoxLayout *HLayout2;
    QHBoxLayout *HLayout3;
    QVBoxLayout *VLayout1;

private:
    void initUI();
    void initSignal();

private slots:
    void on_chkBox_Bold_clicked(bool check);
    void on_chkBox_Italic_clicked(bool check);
    void on_chkBox_Underline_clicked(bool check);
    void on_radioButtonClick();

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();
};
#endif // MAINDIALOG_H
