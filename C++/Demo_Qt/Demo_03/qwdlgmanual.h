#ifndef QWDLGMANUAL_H
#define QWDLGMANUAL_H

#include <QDialog>
#include <QCheckBox>
#include <QRadioButton>
#include <QPlainTextEdit>
#include <QPushButton>

class QWDlgManual : public QDialog
{
    Q_OBJECT

private:
    QCheckBox* chkBoxUnder;
    QCheckBox* chkBoxItalic;
    QCheckBox* chkBoxBold;
    QRadioButton* rBtnBlack;
    QRadioButton* rBtnBlue;
    QRadioButton* rBtnRed;
    QPlainTextEdit* txtEdit;
    QPushButton* btnOK;
    QPushButton* btnCancel;
    QPushButton* btnClose;

    void initUI();
    void initSignalSlots();

private slots:
    void on_click_chkBox();

public:
    QWDlgManual(QWidget *parent = nullptr);
    ~QWDlgManual();
};
#endif // QWDLGMANUAL_H
