#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <QProgressBar>
#include <QLabel>
#include <QSpinBox>
#include <QFontComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class mainWin; }
QT_END_NAMESPACE

class mainWin : public QMainWindow
{
    Q_OBJECT

public:
    mainWin(QWidget *parent = nullptr);
    ~mainWin();

    void initUI();
    void initSIGNAL();

private slots:
    void on_actionBold_triggered(bool checked);
    void customSLOT_spinBoxFontSize_valueChanged(int fontSize);
    void customSLOT_fontComb_FontFamily_indexChanged(QString fontFamily);

private:
    Ui::mainWin *ui;

private:
    QLabel *label1;
    QProgressBar *progressBar;
    QSpinBox *spinBox_FontSize;
    QFontComboBox *fontComb_FontFamily;
};
#endif // MAINWIN_H
