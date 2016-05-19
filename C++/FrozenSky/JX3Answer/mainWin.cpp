#include "mainWin.h"
#include "chinesetopinyin.h"
#include <QFont>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <qt_windows.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("剑网三科举答题器(离线版)");
    //this->setWindowFlags(Qt::WindowStaysOnTopHint);
    SearchLine = new QLineEdit(this);
    SearchLine->setFont(QFont("微软雅黑", 15));
    SearchLine->setFixedHeight(30);
    //SearchLine->setInputMask(">AAAAAAAAAAAAAAAAAAAAAAAA");
    ResultTextEdit = new QTextEdit(this);
    ResultTextEdit->setFont(QFont("Microsoft YaHei", 11));
    HelpBtn = new QPushButton("帮助", this);
    HelpBtn->setFixedHeight(30);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(SearchLine, 0, 0, 1, 6);
    layout->addWidget(HelpBtn, 0, 6, 1, 1);
    layout->addWidget(ResultTextEdit, 1, 0, 1, 7);

    dbonline = false;
    if(!linkToDatabase())
    {
        QMessageBox::information(this, "Error", "无法连接题库！请检查软件目录。");
    }
    else
    {
        qDebug() << "DB Online";
        dbonline = true;
    }

    connect(SearchLine, SIGNAL(textChanged(QString)), this, SLOT(onSearchChange()));
    connect(HelpBtn, SIGNAL(clicked(bool)), this, SLOT(onHelpBtn()));
}

Widget::~Widget()
{

}

bool Widget::linkToDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dbQA_JX3.db");
    if( !db.open() )
    {
        return false;
    }
    return true;
}

void Widget::onSearchChange()
{
    QString searchStr = SearchLine->text();
    //键入3个字符以上才会启动查询
    if(searchStr.length() > 2)
    {
        ResultTextEdit->clear();
        QSqlQuery query;
        QString sqlStr;
        sqlStr.sprintf("SELECT * FROM qatable WHERE pyquestion LIKE '%%%s%%'", searchStr.toStdString().data());
        query.exec(sqlStr);
        QStringList filter;//用于过滤重复的问题
        filter.clear();
        while(query.next())
        {            
            //获得一条记录
            //value0 - pyquestion
            //value1 - question
            //value2 - answer
            QString value0 = query.value(0).toString();
            QString value1 = query.value(1).toString();
            QString value2 = query.value(2).toString();

            //过滤重复的问题
            if(isRepeatQA(filter, value1) == true)
            {
                continue;
            }
            else
            {
                filter.push_back(value1);
                QString tempStr;
                //Update 2016-05-19
                //匹配字符串染色
                int searchLength = searchStr.length();
                int questionLength = value1.length();
                qDebug() << searchLength << " " << questionLength << endl;
                tempStr.sprintf("%s   【 %s 】", value1.toStdString().data(), value2.toStdString().data());
                ResultTextEdit->append(tempStr);
                ResultTextEdit->append("");
            }
        }
    }
}

bool Widget::isRepeatQA(QStringList &list, QString &value)
{
    for(int i = 0; i < list.size(); i++)
    {
        if(value == list.at(i))
        {
            //qDebug() << "检测到重复的问题";
            return true;
        }
    }
    return false;
}

void Widget::onHelpBtn()
{
    QMessageBox::information(this, "小TIPS", "这是一只萌萌的帮助。\n别看它只是一只帮助，但是它很努力，你知道吗。");
}

bool Widget::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
    Q_UNUSED(result);
    if(eventType == "windows_generic_MSG")
    {
        MSG* msg = (MSG*)message;
        if(msg->message == WM_ACTIVATE)
        {
            SearchLine->selectAll();
            SearchLine->setFocus();
            //SearchLine->clear();
            //qDebug() << "nativeEvent";
        }
    }
    return false;
}
