#include "mainwindow.h"
#include "chinesetopinyin.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle(tr("QT SQLite Control"));
    showDBDriversBtn = new QPushButton(tr("Update Database"),this);
    openDBBtn = new QPushButton(tr("Open Database"),this);
    createTableBtn = new QPushButton(tr("Create Table"), this);
    importFileBtn = new QPushButton(tr("Import File"), this);
    selectTableBtn = new QPushButton(tr("Select Table"), this);
    mainTextEdit = new QTextEdit(this);
    mainTextEdit->setFontPointSize(15.0f);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(showDBDriversBtn,0,0,1,1);
    layout->addWidget(openDBBtn,0,1,1,1);
    layout->addWidget(createTableBtn,0,2,1,1);
    layout->addWidget(importFileBtn,0,3,1,1);
    layout->addWidget(selectTableBtn,0,4,1,1);
    layout->addWidget(mainTextEdit,1,0,1,5);

    connect(showDBDriversBtn,SIGNAL(clicked(bool)),this,SLOT(updateDB()));
    connect(openDBBtn,SIGNAL(clicked(bool)),this,SLOT(openDB()));
    connect(createTableBtn,SIGNAL(clicked(bool)),this,SLOT(createTableQuery()));
    connect(importFileBtn,SIGNAL(clicked(bool)),this,SLOT(importFile()));
    connect(selectTableBtn,SIGNAL(clicked(bool)),this,SLOT(selectTableQuery()));
}

Widget::~Widget()
{

}

//查看数据库支持列表
void Widget::showDBDrivers()
{
    mainTextEdit->append("Available Drivers:");
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
    //添加数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名称
    db.setDatabaseName("dbQA.db");
    //打开数据库
    if( !db.open() )
    {
        QMessageBox::information(this, "Error", "数据库连接失败！");
        return false;
    }
    QMessageBox::information(this, "Success", "数据库已连接！");
    return true;
}

bool Widget::createTableQuery()
{
    //Sql语句
    QSqlQuery query;
    //query.exec("CREATE TABLE qatable(id varchar, pyquestion varchar, question varchar, answer varchar)");
    //Update_20160306:增加了正确答案序号列
    query.exec("CREATE TABLE qatable(id varchar, pyquestion varchar, question varchar, answer varchar, rightnum varchar)");
    QMessageBox::information(this, "Create", "数据表创建完成！");
    return true;
}

void Widget::selectTableQuery()
{
    QSqlQuery query;
    query.exec("SELECT * FROM qatable where pyquestion like '%YLHD%'");
    mainTextEdit->clear();
    //query.next()指向查找到的第一条记录，然后每次后移一条记录
    while(query.next())
    {
        QString value0 = query.value(0).toString();
        QString value1 = query.value(1).toString();
        QString value2 = query.value(2).toString();
        QString value3 = query.value(3).toString();
        QString value4 = query.value(4).toString();
        QString tempStr;
        tempStr.sprintf("| %s | %s | %s | %s | %s |",
                        value0.toStdString().data(),
                        value1.toStdString().data(),
                        value2.toStdString().data(),
                        value3.toStdString().data(),
                        value4.toStdString().data());
        mainTextEdit->append(tempStr);
    }
}

void Widget::insertToDB(QString id, QString pyquestion, QString question, QString answer, QString rightnum)
{
    QSqlQuery query;
    QString sqlstr;
    sqlstr.sprintf("INSERT INTO qatable VALUES('%s', '%s', '%s', '%s', '%s')",
                   id.toStdString().data(),
                   pyquestion.toStdString().data(),
                   question.toStdString().data(),
                   answer.toStdString().data(),
                   rightnum.toStdString().data());

    query.exec(sqlstr);
}

//导入数据文件生成数据库
void Widget::importFile()
{
    //获得文件路径
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open DataFile"), QDir::currentPath());
    if(filePath.isEmpty())
    {
        return;
    }
    mainTextEdit->append(filePath);
    //打开文件
    QFile file;
    file.setFileName(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "Error", "打开文件出错！");
        return;
    }
    QTextStream *stream = new QTextStream(&file);
    QString tempStr = stream->readLine();
    while(!tempStr.isEmpty())
    {
        QStringList tempList = tempStr.split("|");
        const QString id = tempList.at(0);
        const QString question = tempList.at(3);
        QString rightNum = tempList.at(4);
        QString answerList = tempList.at(5);
        int num = rightNum.toInt();
        QStringList tempList1 = answerList.split(",");
        const QString answer = tempList1.at(num-1);

        //mainTextEdit->append(id + " " + ToChineseSpell(question) + " " + question + " " + answer);
        insertToDB(id, ToChineseSpell(question), question, answer, rightNum);

        tempStr = stream->readLine();
    }
    file.close();
    QMessageBox::information(this, "Import", "导入数据库完成！");
}

//Update_20160306:新建更新数据库函数
void Widget::updateDB()
{
    // 2016-03-06
    // updateSql(147633, 147772, 16561);
    // updateSql(81817, 82096, 16281);

    // 2016-03-09
    updateSql(148633, 148772, 17561);
    updateSql(82817, 83096, 17281);
}

void Widget::updateSql(int idStart, int idEnd, int newid)
{
    QSqlQuery query;
    QString sqlstr;
    for(int i = idStart; i<= idEnd; i++)
    {
        sqlstr.sprintf("UPDATE qatable SET id = '%d' WHERE id = '%d'", newid, i);
        newid++;
        query.exec(sqlstr);
    }
}
