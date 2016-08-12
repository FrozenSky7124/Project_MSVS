#ifndef DETAIL_H
#define DETAIL_H

#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>

class Detail : public QWidget
{
    Q_OBJECT
public:
    explicit Detail(QWidget *parent = 0);

private:
    QLabel *NationalLabel;
    QComboBox *NationalComboBox;
    QLabel *ProvinceLabel;
    QComboBox *ProvinceComboBox;
    QLabel *CityLabel;
    QLineEdit *CityLineEdit;
    QLabel *IntroductLabel;
    QTextEdit *IntroductTextEdit;
    QGridLayout *mainLayout;

signals:

public slots:
};

#endif // DETAIL_H
