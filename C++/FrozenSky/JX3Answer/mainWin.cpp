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

#define Author "FrozenSky"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("剑网三科举小助手");
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

    //托盘区图标
    trayicon = new QSystemTrayIcon(this);
    trayicon->setIcon(QIcon(":/icon.ico"));

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
    connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
}

Widget::~Widget()
{

}

bool Widget::linkToDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dbQA_JX3.db");
    //db.setDatabaseName("qrc:/dbQA_JX3.db");
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
            QString qanda = value1 + value2;
            //过滤重复的问题
            if(isRepeatQA(filter, qanda) == true)
            {
                continue;
            }
            else
            {
                filter.push_back(qanda);
                QString tempStr;
                //Update 2016-05-19
                //匹配字符串染色
                int searchLength = searchStr.length();
                int questionLength = value1.length();
                int posKeyWords = value0.indexOf(searchStr.toUpper());

                int pCur = posKeyWords;
                int pLen = searchLength;
                while(pCur <= questionLength - searchLength)
                {
                    if(ToChineseSpell(value1.mid(pCur, pLen)) == searchStr.toUpper())
                    {
                        pLen++;
                        if(ToChineseSpell(value1.mid(pCur, pLen)) == searchStr.toUpper())
                            break;
                        else
                        {
                            pLen--;
                            break;
                        }
                    }
                    else
                    {
                        pLen++;
                        if(pCur+pLen >= questionLength)
                        {
                            pLen = searchLength;
                            pCur++;
                        }
                    }
                }
                //qDebug() << "KeyLength=" << searchLength << " KeyPos=" << posKeyWords << " KeyLen=" << pLen ;
                tempStr.sprintf("%s<font color=red>%s</font>%s   <font color=blue>  %s </font>",
                                value1.mid(0, pCur).toStdString().data(),
                                value1.mid(pCur, pLen).toStdString().data(),
                                value1.mid(pCur+pLen).toStdString().data(),
                                value2.toStdString().data());

                //tempStr.sprintf("%s   <font color=blue>  %s  </font>", value1.mid(0).toStdString().data(), value2.toStdString().data());
                //tempStr.sprintf("%s   <font color=blue>  %s  </font>", value1.toStdString().data(), value2.toStdString().data());
                //Update 2016-05-20 End.
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
    QMessageBox::information(this, "小TIPS", "<font color=#66ccff size=\"8\">这是一只萌萌的帮助 O(∩_∩)O~\n</font><br><font color=black size=\"5\">别看它只是一只帮助，但是它很努力，你知道吗！</font><br>\n"
                                   "\n<br>"
                                   "<font color=black size=\"5\">简化搜索的操作，只需要在搜索框里输入题目中连续的某段文字的<font color=red>拼音首字母</font>即可快速索引，题目中的标点符号(比如常见的书名号《》)不用输入，可以跳过标点输入拼音首字母。搜索的关键文字将以红色高亮显示，答案则以蓝色高亮显示。<br>\n"
                                   "\n<br>"
                                   "<font color=red>窗口总在最前</font>，并且现在可以<font color=red>最小化到托盘区</font>啦，从其他窗口切换至小助手将会自动选中搜索框中的内容，答题后直接搜索下一题吧！\n<br>"
                                   "\n<br>"
                                   "题库已经优化过啦，<font color=red>不会出现一大堆相同的答案啦</font>！所以如果搜索出了看似相同的答案，就要仔细审题了，因为其中定有猫腻！</font><br>");
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

//最小化到托盘区
void Widget::changeEvent(QEvent *event)
{
    if((event->type()==QEvent::WindowStateChange)&&this->isMinimized())
    {
        trayicon->show();
        this->hide();
    }
}

//托盘区图标响应函数
void Widget::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if(this->isHidden())
        {
            this->show();
            this->setWindowState(Qt::WindowActive);
            this->activateWindow();
            SearchLine->selectAll();
            SearchLine->setFocus();
            trayicon->hide();
        }
        break;
    default:
        break;
    }
}
