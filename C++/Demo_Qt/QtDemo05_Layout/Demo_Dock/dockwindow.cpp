#include "dockwindow.h"
#include <QTextEdit>
#include <QDockWidget>

DockWindow::DockWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(tr("DockWindows"));
    QTextEdit *te = new QTextEdit(tr("Main Window"), this);
    te->setAlignment(Qt::AlignCenter);
    this->setCentralWidget(te); //将QTextEdit设置为主窗口的中央框体
    //停靠窗口1
    QDockWidget *dock = new QDockWidget(tr("DockWindow1"), this);
    dock->setFeatures(QDockWidget::DockWidgetMovable); //设置为可移动
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea); //设置停靠窗口允许停靠的位置
    QTextEdit *te1 = new QTextEdit();
    te1->setText(tr("Window1, The dock widget can be moved between docks by the user."));
    dock->setWidget(te1);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    //停靠窗口2
    dock = new QDockWidget(tr("DockWindow2"), this);
    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable); //可关闭可浮动
    QTextEdit *te2 = new QTextEdit();
    te2->setText(tr("Window2, The dock widget can be detached from the main window,""and floated as an independent window, and can be closed."));
    dock->setWidget(te2);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    //停靠窗口3
    dock = new QDockWidget(tr("DockWindow3"), this);
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures); //设置全部特性
    QTextEdit *te3 = new QTextEdit();
    te3->setText(tr("Window3, The dock widget can be closed, moved, and floated."));
    dock->setWidget(te3);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

DockWindow::~DockWindow()
{

}
