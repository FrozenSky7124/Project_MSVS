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
    //天涯明月刀OL
    openDBBtn = new QPushButton(tr("Open Database TY"),this);
    createTableBtn = new QPushButton(tr("Create Table"), this);
    importFileBtn = new QPushButton(tr("Import File"), this);
    updateDBBtn = new QPushButton(tr("Update Database"),this);
    selectTableBtn = new QPushButton(tr("Select Table"), this);
    //剑侠情缘网络版三
    openDBBtn1 = new QPushButton(tr("Open Database JX3"), this);
    createTableBtn1 = new QPushButton(tr("Create Table"), this);
    importFileBtn1 = new QPushButton(tr("Import File"), this);
    updateDBBtn1 = new QPushButton(tr("Update Database"),this);
    selectTableBtn1 = new QPushButton(tr("Select Table"), this);
    //主窗口
    mainTextEdit = new QTextEdit(this);
    mainTextEdit->setFontPointSize(12.0f);

    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(openDBBtn,0,0,1,1);
    layout->addWidget(createTableBtn,0,1,1,1);
    layout->addWidget(importFileBtn,0,2,1,1);
    layout->addWidget(updateDBBtn,0,3,1,1);
    layout->addWidget(selectTableBtn,0,4,1,1);

    layout->addWidget(openDBBtn1, 1, 0, 1, 1);
    layout->addWidget(createTableBtn1, 1, 1, 1, 1);
    layout->addWidget(importFileBtn1, 1, 2, 1, 1);
    layout->addWidget(updateDBBtn1, 1, 3, 1, 1);
    layout->addWidget(selectTableBtn1, 1, 4, 1, 1);

    layout->addWidget(mainTextEdit,2,0,1,5);

    connect(openDBBtn,SIGNAL(clicked(bool)),this,SLOT(openDB()));
    connect(createTableBtn,SIGNAL(clicked(bool)),this,SLOT(createTableQuery()));
    connect(importFileBtn,SIGNAL(clicked(bool)),this,SLOT(importFile()));
    connect(updateDBBtn,SIGNAL(clicked(bool)),this,SLOT(updateDB()));
    connect(selectTableBtn,SIGNAL(clicked(bool)),this,SLOT(selectTableQuery()));

    connect(openDBBtn1, SIGNAL(clicked(bool)), this, SLOT(openDB1()));
    connect(createTableBtn1, SIGNAL(clicked(bool)), this, SLOT(createTableQuery1()));
    connect(importFileBtn1, SIGNAL(clicked(bool)), this, SLOT(importFile1()));
    connect(updateDBBtn1, SIGNAL(clicked(bool)), this, SLOT(updateDB1()));
    connect(selectTableBtn1, SIGNAL(clicked(bool)), this, SLOT(selectTableQuery1()));
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

bool Widget::openDB1()
{
    //添加数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名称
    db.setDatabaseName("dbQA_JX3.db");
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

bool Widget::createTableQuery1()
{
    //Sql语句
    QSqlQuery query;
    query.exec("CREATE TABLE qatable(pyquestion varchar, question varchar, answer varchar)");
    QMessageBox::information(this, "Create", "数据表创建完成！");
    return true;
}

void Widget::selectTableQuery()
{
    QSqlQuery query;
    //query.exec("SELECT * FROM qatable where pyquestion like '%YLHD%'");
    query.exec("SELECT * FROM qatable");
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

void Widget::selectTableQuery1()
{
    QSqlQuery query;
    //query.exec("SELECT * FROM qatable where pyquestion like '%YLHD%'");
    query.exec("SELECT * FROM qatable");
    mainTextEdit->clear();
    //query.next()指向查找到的第一条记录，然后每次后移一条记录
    while(query.next())
    {
        QString value0 = query.value(0).toString();
        QString value1 = query.value(1).toString();
        QString value2 = query.value(2).toString();
        QString tempStr;
        tempStr.sprintf("| %s | %s | %s |",
                        value0.toStdString().data(),
                        value1.toStdString().data(),
                        value2.toStdString().data());
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

void Widget::insertToDB1(QString pyquestion, QString question, QString answer)
{
    QSqlQuery query;
    QString sqlstr;
    sqlstr.sprintf("INSERT INTO qatable VALUES('%s', '%s', '%s')",
                   pyquestion.toStdString().data(),
                   question.toStdString().data(),
                   answer.toStdString().data());

    query.exec(sqlstr);
}

void Widget::changeAnswerDB1(QString question, QString answer)
{
    QSqlQuery query;
    QString sqlstr;
    sqlstr.sprintf("UPDATE qatable SET answer = '%s' WHERE question = '%s'",
                   answer.toStdString().data(),
                   question.toStdString().data());
    query.exec(sqlstr);
}

void Widget::changeQuestionDB1(QString questionOLD, QString questionNEW)
{
    QSqlQuery query;
    QString sqlstr1;
    sqlstr1.sprintf("UPDATE qatable SET pyquestion = '%s' WHERE question = '%s'",
                   ToChineseSpell(questionNEW).toStdString().data(),
                   questionOLD.toStdString().data());
    query.exec(sqlstr1);
    QString sqlstr2;
    sqlstr2.sprintf("UPDATE qatable SET question = '%s' WHERE question = '%s'",
                   questionNEW.toStdString().data(),
                   questionOLD.toStdString().data());
    query.exec(sqlstr2);
}

//导入数据文件生成数据库（天涯明月刀OL）
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

//导入数据文件生成数据库（剑侠情缘网络版三）
void Widget::importFile1()
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
        QStringList tempList = tempStr.split("\"");
        const QString question = tempList.at(1);
        const QString answer = tempList.at(3);
        QStringList tempList2 = question.split("单选题：");//再次分割题目
        const QString question2 = tempList2.at(1);
        mainTextEdit->append(ToChineseSpell(question2) + " " + question2 + " " + answer);
        insertToDB1(ToChineseSpell(question2), question2, answer);
        tempStr = stream->readLine();
    }
    file.close();
    QMessageBox::information(this, "Import", "导入数据库完成！");
}

//Update_20160306:新建更新数据库函数
void Widget::updateDB()
{
    // 2016-03-06
    // updateSql_TY(147633, 147772, 16561);
    // updateSql_TY(81817, 82096, 16281);

    // 2016-03-09
    //updateSql_TY(148633, 148772, 17561);
    //updateSql_TY(82817, 83096, 17281);

    // 2016-06-29
    //71817 ~ 72096 与 77817  ~  78096 ***** 12281=77817 *****
    //137633~137772 与 143633 ~ 143772 ***** 12561=143633 *****
    //updateSql_TY(77817, 78096, 12281);
    //updateSql_TY(143633, 143772, 12561);
}

void Widget::updateDB1()
{
    //插入问答数据：insertToDB1(QString pyquestion, QString question, QString answer)
    //根据问题修改答案：changeAnswerDB1(QString question, QString answer)
    //TEST
    //changeAnswerDB1("金生____,玉出____。", "丽水、昆冈");

    //2016-04-29
    //changeAnswerDB1("目前游戏中的阅读等级最高为多少级？", "95级");
    //changeAnswerDB1("春秋时期吴国的忠臣伍子胥本为哪国人？", "楚国");
    //changeAnswerDB1("《佛藏拾珠·卷七》这套书中有多少本是蓝色品质的？", "三本");
    //changeAnswerDB1("《嫦娥奔月》的故事中，后羿交给嫦娥珍藏的“不死药”是他向谁求得的？", "王母娘娘");
    //changeAnswerDB1("下面哪个不是“万花七圣“之一？", "乌有先生");
    //changeQuestionDB1("伟伟，房子，和粽子为试验对毒物的抵抗力，决定各选一种毒物服食，他们三人分别吃了鹤顶红，断肠草和一钩吻，他们即将中毒死去了，这是粽子从怀里掏出了六种解毒药物，炭灰，碱水，催吐汤，绿豆，金银花，甘草，他们之中谁不会死？",
    //                  "伟伟，房子，和粽子为试验对毒物的抵抗力，决定各选一种毒物服食，他们三人分别吃了鹤顶红，断肠草和一钩吻，他们即将中毒死去了，这是粽子从怀里掏出了六种解毒药物，炭灰，碱水，催吐汤，绿豆，金银花，甘草，他们之中谁不会死？");
    //insertToDB1(ToChineseSpell("天策府的宣威将军曹雪阳是______的后人？"), "天策府的宣威将军曹雪阳是______的后人？", "曹孟德");
    //insertToDB1(ToChineseSpell("狼牙军“山狼”曹将军和天策府宣威将军曹雪阳的关系是______。"), "狼牙军“山狼”曹将军和天策府宣威将军曹雪阳的关系是______。", "兄妹");

    //2016-05-15
    //insertToDB1(ToChineseSpell("题目：以下哪组技能全部都是减伤技能？"), "题目：以下哪组技能全部都是减伤技能？", "守如山，天地低昂，袖纳乾坤，春泥护花");
    QMessageBox::information(this, "Update dbQA_JX3", "数据库更新完成！");
}

void Widget::updateSql_TY(int idStart, int idEnd, int newid)
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
