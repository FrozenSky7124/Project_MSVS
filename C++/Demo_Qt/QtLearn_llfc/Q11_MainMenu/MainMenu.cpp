#include "MainMenu.h"
#include "./ui_MainMenu.h"
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QActionGroup>
#include "MyAction.h"
#include <QToolButton>
#include <QSpinBox>
#include <QLabel>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    QMenu * pMenuEdit = ui->menubar->addMenu(tr("Edit(&E)"));
    pMenuEdit->addSeparator();
    QAction * pActionOpen = pMenuEdit->addAction(QIcon(":/res/edit.png"), tr("OpenFile(&O)"));
    pActionOpen->setShortcut(QKeySequence("Ctrl+O"));
    QObject::connect(pActionOpen, &QAction::triggered, this, &MainMenu::on_actionEdit_Open_triggered);
    QActionGroup * pAG1 = new QActionGroup(this);
    QAction *pActL = pAG1->addAction(tr("左对齐(&L)"));
    pActL->setCheckable(true);
    QAction *pActM = pAG1->addAction(tr("居中(&M)"));
    pActM->setCheckable(true);
    QAction *pActR = pAG1->addAction(tr("右对齐(&R)"));
    pActR->setCheckable(true);
    pMenuEdit->addSeparator();
    pMenuEdit->addAction(pActL);
    pMenuEdit->addAction(pActM);
    pMenuEdit->addAction(pActR);
    pMenuEdit->addSeparator();
    MyAction * pMyAct = new MyAction(this);
    pMenuEdit->addAction(pMyAct);
    QObject::connect(pMyAct, &MyAction::getText, this, &MainMenu::setText);

    QToolButton * pToolBtn = new QToolButton(this);
    pToolBtn->setText(tr("Color"));
    ui->toolBar->addWidget(pToolBtn);
    QMenu * pMenuColor = new QMenu(this);
    pMenuColor->addAction(tr("RED"));
    pMenuColor->addAction(tr("YELLOW"));
    pToolBtn->setMenu(pMenuColor);
    pToolBtn->setPopupMode(QToolButton::MenuButtonPopup);
    ui->toolBar->addSeparator();

    ui->statusbar->showMessage(tr("ABC"), 3000);
    QLabel * pLabelStatus = new QLabel(this);
    pLabelStatus->setFrameStyle(QFrame::Box | QFrame::Sunken);
    pLabelStatus->setText(tr("DemoQT llfc 11"));
    ui->statusbar->addPermanentWidget(pLabelStatus);
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

void MainMenu::on_actionEdit_Open_triggered()
{
    qDebug() << "menuBar.menuEdit.actionOpen.triggered" << Qt::endl;
}

void MainMenu::setText(const QString& str)
{
    qDebug() << str << Qt::endl;
}
