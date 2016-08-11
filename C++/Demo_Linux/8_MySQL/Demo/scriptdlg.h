#ifndef SCRIPTDLG_H
#define SCRIPTDLG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QString>

class scriptDlg : public QDialog
{
    Q_OBJECT
public:
    scriptDlg();
    QString SQL;

    bool islogin;
private:

    QLabel *label0;
    QTextEdit *textEditSQL;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

private slots:
    void okBtnOnclick();
    void cancelBtnOnclick();
};

#endif // SCRIPTDLG_H
