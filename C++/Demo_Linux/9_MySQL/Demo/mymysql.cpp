#include "mymysql.h"
#include <QMessageBox>
#include <string.h>


mymysql::mymysql()
{
    mysql_init(&mysql);
    connection = NULL;
    memset(buf, 0, sizeof(buf));
}

const char *mymysql::geterror()
{
    return buf;
}

int mymysql::sql_connect(const char *Hostname, const char *User, const char *Password, const char *DBName)
{
    connection = mysql_real_connect(&mysql, Hostname, User, Password, DBName, 0, 0, 0);
    if (connection == NULL)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        //QMessageBox::information(0, "",mysql_error(&mysql));
        return -1;
    }
    else
    {
        mysql_query(connection, "SET NAMES utf8");
        return 0;
    }
}

void mymysql::sql_disconnect()
{
    if (connection)
    {
        mysql_close(connection);
        connection = NULL;
    }
}

int mymysql::sql_exec(const char *SQL)
{
    if(mysql_query(connection, SQL) != 0)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }
    return 0;
}

int mymysql::sql_open(const char *SQL, QStandardItemModel **p)
{
    if(mysql_query(connection, SQL) != 0)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(connection);
    if (result == NULL)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }

    int rowcount = mysql_affected_rows(connection);//SQL语句执行后返回多少行
    int fieldcount = mysql_field_count(connection);//SQL语句执行后返回多少列
    *p = new QStandardItemModel(rowcount, fieldcount);//动态创建Model

    MYSQL_FIELD *field;
    int i = 0;
    int j = 0;
    for (i=0;i<fieldcount;i++)
    {
        field = mysql_fetch_field(result);//读取字段
        (*p)->setHeaderData(i, Qt::Horizontal, field->name);
    }
    for(i=0;i<rowcount;i++)
    {
        MYSQL_ROW row = mysql_fetch_row(result);//从SQL结果集中获取一行
        for(j=0;j<fieldcount;j++)
        {
            (*p)->setData((*p)->index(i, j, QModelIndex()), row[j]);
        }
    }


    mysql_free_result(result);
    return 0;
}

