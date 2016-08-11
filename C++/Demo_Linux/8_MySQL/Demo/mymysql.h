#ifndef MYMYSQL_H
#define MYMYSQL_H

#include <windows.h>
#include "D:/MySQL/include/mysql.h"
#include <QStandardItemModel>

class mymysql
{
public:
    mymysql();

    const char *geterror();
    int sql_connect(const char *Hostname, const char *User, const char *Password, const char *DBName);
    void sql_disconnect();
    int sql_exec(const char *SQL);
    int sql_open(const char *SQL, QStandardItemModel **p);

private:
    MYSQL *connection;
    MYSQL mysql;
    char buf[1024];
};

#endif // MYMYSQL_H
