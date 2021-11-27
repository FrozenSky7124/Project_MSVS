#ifndef T02_DIALOG_H
#define T02_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class T02_Dialog; }
QT_END_NAMESPACE

class T02_Dialog : public QDialog
{
    Q_OBJECT

public:
    T02_Dialog(QWidget *parent = nullptr);
    ~T02_Dialog();

private slots:
    void on_checkBox_Bold_clicked(bool checked);

    void on_checkBox_Italic_clicked(bool checked);

    void on_checkBox_Underline_clicked(bool checked);

    void on_radioButtonClick();

private:
    Ui::T02_Dialog *ui;
};
#endif // T02_DIALOG_H
