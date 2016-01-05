#include "mainwindow.h"
#include "logindlg.h"
#include "scriptdlg.h"
#include <QMessageBox>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(tr("NTSS控制中心"));
    this->setWindowIcon(QIcon("img_Icon.jpg"));//设置图标

    mdiArea = new QMdiArea;//实例化多文档界面组件
    this->setCentralWidget(mdiArea);//设置主窗体中央控件为多文档界面组件mdiArea
    mdiArea->setBackground(Qt::NoBrush);
    mdiArea->setStyleSheet("QMdiArea{background-image:url(img_background.jpg);background-attachment:fixed;background-repeat:no-repeat;background-position:center;}");

    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//滚动条
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //event->accept();//接受退出
    //event->ignore();//不接受退出
    //closeEvent函数在Widget退出之前调用
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, "提示", "是否退出控制中心？", QMessageBox::Yes | QMessageBox::No);
    if(button == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::showView()
{
    QStandardItemModel *model = new QStandardItemModel(5, 3);
/*
    model->setHeaderData(0, Qt::Horizontal, "姓名");
    model->setHeaderData(1, Qt::Horizontal, "性别");
    model->setHeaderData(2, Qt::Horizontal, "年龄");
*/
    model->setData(model->index(0, 0, QModelIndex()), "啦啦");

    QTableView *view1 = new QTableView;
    view1->setAttribute(Qt::WA_DeleteOnClose);
    view1->setStyleSheet("background-image:url(img_Sub.jpg);");
    mdiArea->addSubWindow(view1);
    view1->setModel(model);
    view1->show();
    mdiArea->activeSubWindow()->resize(width()-100, height()-100);
}

void MainWindow::showSub()
{
    QWidget *w1 = new QWidget;
    w1->setAttribute(Qt::WA_DeleteOnClose);//关闭时自动施放资源
    mdiArea->addSubWindow(w1);//加入子窗口
    w1->setWindowTitle(tr("Poi~"));
    w1->setStyleSheet("background-image:url(img_Sub.jpg);");
    w1->show();
    mdiArea->activeSubWindow()->resize(width()-100, height()-100);
}

void MainWindow::createMenus()
{
    adminMenu = this->menuBar()->addMenu(tr("管理"));
    adminMenu->addAction(loginAction);
    adminMenu->addAction(logoutAction);
    adminMenu->addSeparator();//插入菜单分隔符
    adminMenu->addAction(exitAction);

    dataMenu = this->menuBar()->addMenu(tr("数据"));
    dataMenu->addSeparator();
    dataMenu->addAction(scriptAction);

    windowMenu = this->menuBar()->addMenu(tr("窗口"));
    windowMenu->addAction(cascadeAction);
    windowMenu->addAction(tileAction);

    helpMenu = this->menuBar()->addMenu(tr("帮助"));
    helpMenu->addAction(helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
}

void MainWindow::createActions()
{
    loginAction = new QAction(tr("登录"),this);
    loginAction->setShortcut(tr("Ctrl+U"));
    connect(loginAction, SIGNAL(triggered(bool)), this,SLOT(on_login()));

    logoutAction = new QAction(tr("注销"),this);
    logoutAction->setShortcut(tr("Ctrl+B"));
    connect(logoutAction, SIGNAL(triggered(bool)), this,SLOT(on_logout()));

    exitAction = new QAction(tr("退出"),this);
    exitAction->setShortcut(tr("Ctrl+W"));
    connect(exitAction, SIGNAL(triggered(bool)), this,SLOT(on_exit()));

    scriptAction = new QAction(tr("执行脚本"),this);
    scriptAction->setShortcut(tr("Ctrl+P"));
    scriptAction->setEnabled(false);//设置初始化时，执行脚本菜单为不可用
    connect(scriptAction, SIGNAL(triggered(bool)), this,SLOT(on_script()));

    cascadeAction = new QAction(tr("层叠"),this);
    cascadeAction->setShortcut(tr("Ctrl+Q"));
    connect(cascadeAction, SIGNAL(triggered(bool)), this,SLOT(cascadeSubWindows()));

    tileAction = new QAction(tr("并列"),this);
    tileAction->setShortcut(tr("Ctrl+R"));
    connect(tileAction, SIGNAL(triggered(bool)), this,SLOT(tileSubWindows()));

    helpAction = new QAction(tr("帮助"),this);
    helpAction->setShortcut(tr("Ctrl+S"));
    connect(helpAction, SIGNAL(triggered(bool)), this,SLOT(on_help()));

    aboutAction = new QAction(tr("关于"),this);
    aboutAction->setShortcut(tr("Ctrl+T"));
    connect(aboutAction, SIGNAL(triggered(bool)), this,SLOT(on_about()));
}

//Function SLOTS
void MainWindow::on_login()
{
    loginDlg dlg;
    dlg.exec();//调用EXEC产生模式对话框
    if(dlg.islogin == true)
    {
        int res = db.sql_connect(dlg.hostip.toStdString().data(),
                       dlg.userid.toStdString().data(),
                       dlg.passwd.toStdString().data(),
                       dlg.dbname.toStdString().data());
        if (res == -1)
        {
            QMessageBox::information(this, "登录失败！", db.geterror());
        }
        else
        {
            scriptAction->setEnabled(true);
            QMessageBox::information(this, "", "登录成功！");
        }
    }
}

void MainWindow::on_logout()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, "提示", "是否注销？", QMessageBox::Yes | QMessageBox::No);
    if(button == QMessageBox::Yes)
    {
        db.sql_disconnect();
        scriptAction->setEnabled(false);
    }
    else
    {

    }
}

void MainWindow::on_exit()
{
    this->close();
}

void MainWindow::script_msg(const char *SQL)
{
    int res = 0;
    if ((strncmp(SQL, "SELECT", 6) == 0)||(strncmp(SQL, "select", 6) == 0))//命令若为SELECT
    {
        QStandardItemModel *model = NULL;
        res = db.sql_open(SQL, &model);

        QTableView *view1 = new QTableView;
        view1->setAttribute(Qt::WA_DeleteOnClose);
        view1->setStyleSheet("background-image:url(img_Sub.jpg);");
        mdiArea->addSubWindow(view1);
        view1->setModel(model);
        view1->show();
        mdiArea->activeSubWindow()->resize(width()-100, height()-100);
    }
    else
    {
        res = db.sql_exec(SQL);
    }
    if (res == -1)
    {
        QMessageBox::information(this, "执行失败！", db.geterror());
    }
    else
    {
        QMessageBox::information(this, "提示", "执行成功！");
    }
}

void MainWindow::on_script()
{
    scriptDlg dlg;
    dlg.exec();
    if (dlg.islogin)
    {
        script_msg(dlg.SQL.toStdString().data());
    }
    //showSub();
    //showView();
}

void MainWindow::cascadeSubWindows()
{
    mdiArea->cascadeSubWindows();//调用QT层叠函数
}

void MainWindow::tileSubWindows()
{
    mdiArea->tileSubWindows();//调用QT并列函数
}

void MainWindow::on_help()
{
    QMessageBox::information(this, "帮助", "还没有帮助 poi~");
}

void MainWindow::on_about()
{
    QMessageBox::about(this, "关于", "版权所有！随便盗版！");
}
