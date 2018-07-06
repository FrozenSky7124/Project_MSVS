#include "mainwindow.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // Widget UI
    setWindowTitle(tr("QT Tycho2 SQL Control"));
    btnOpenDB = new QPushButton(tr("Open Database"), this);
    mainTextEdit = new QTextEdit(this);
    mainTextEdit->setFont(QFont(tr("mononoki"), 12));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(btnOpenDB, 0, 0, 1, 1);
    layout->addWidget(mainTextEdit, 1, 0, 1, 1);

    // Bind Slots
    connect(btnOpenDB, SIGNAL(clicked(bool)), this, SLOT(openDB()));

    // Show Database Support Drivers
    showDBDrivers();
}

Widget::~Widget()
{

}


void Widget::showDBDrivers()
{
    mainTextEdit->clear();
    mainTextEdit->append("Database Support Drivers:\n");
    QStringList drivers = QSqlDatabase::drivers();
    for (int i = 0; i<drivers.size(); i++)
    {
        QString driver;
        driver = drivers.at(i);
        mainTextEdit->append(driver);
    }
}


bool Widget::openDB()
{
    mainTextEdit->clear();
    mainTextEdit->append("btnOpenDB Clicked.\n");
    return true;
}
