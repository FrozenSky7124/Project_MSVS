#include "MainMenu.h"
#include "./ui_MainMenu.h"
#include <QTextEdit>
#include <QMdiSubWindow>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_actionNew_N_triggered()
{
    QTextEdit *pTextEdit = new QTextEdit(this);
    auto childWindow = ui->mdiArea->addSubWindow(pTextEdit);
    childWindow->setWindowTitle(tr("TextEdit"));
    childWindow->show();
}

void MainMenu::on_actionDock_D_triggered()
{
    ui->dock1->show();
}

