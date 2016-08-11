#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <mysql/mysql.h>

MYSQL mysql;
MYSQL *connection;

struct termios oldterm;

void setstty()//设置退格键不回显
{
	struct termios term;//定义一个Termios结构
	if (tcgetattr(STDIN_FILENO, &term) == -1) //得到系统Termios的设置
	{
		printf("tcgetattr error is %s\n", strerror(errno));
		return;
	}
	oldterm = term;//保留系统原始Termios设置

	/*
	term.c_lflag &= ~ICANON;//取消ICANON选项（不规范输入）
	term.c_lflag |= ICANON;//设置ICANON选项（规范输入）
	term.c_cc[VERASE] = '\b';//将退格键修改为'\b'
	*/
	term.c_cc[VERASE] = '\b';
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)//设置系统Termios
	{
		printf("tcsetattr error is %s\n", strerror(errno));
		return;
	}
	return;
}

int deletename()
{
	char name[1024];
	memset(name, 0, sizeof(name));
	read(STDIN_FILENO, name, sizeof(name));
	name[strlen(name) - 1] = 0;

	char SQL[1024];
	memset(SQL, 0, sizeof(SQL));
	sprintf(SQL, "DELETE FROM pal_people WHERE name = '%s'", name);

	if (mysql_query(connection, SQL) != 0)
	{
		printf("Error in mysql_query:DELETE.\n%s\n", mysql_error(&mysql));
		return -1;
	}
	return 0;
}

int insertname()
{
	char SQL[1024];
	char tip[512];
	memset(tip, 0, sizeof(tip));
	sprintf(tip, "%s", "Enter Name:");
	write(STDOUT_FILENO, tip, strlen(tip));
	char name[50];
	memset(name, 0, sizeof(name));
	read(STDIN_FILENO, name, sizeof(name));
	name[strlen(name) - 1] = 0;

	memset(tip, 0, sizeof(tip));
	sprintf(tip, "%s", "Enter Sex:");
	write(STDOUT_FILENO, tip, strlen(tip));
	char sex[10];
	memset(sex, 0, sizeof(sex));
	read(STDIN_FILENO, sex, sizeof(sex));
	sex[strlen(sex) - 1] = 0;

	memset(tip, 0, sizeof(tip));
	sprintf(tip, "%s", "Enter Age:");
	write(STDOUT_FILENO, tip, strlen(tip));
	char age[10];
	memset(age, 0, sizeof(age));
	read(STDIN_FILENO, age, sizeof(age));
	age[strlen(age) - 1] = 0;

	memset(tip, 0, sizeof(tip));
	sprintf(tip, "%s", "Enter Race:");
	write(STDOUT_FILENO, tip, strlen(tip));
	char race[50];
	memset(race, 0, sizeof(race));
	read(STDIN_FILENO, race, sizeof(race));
	race[strlen(race) - 1] = 0;

	memset(tip, 0, sizeof(tip));
	sprintf(tip, "%s", "Enter School:");
	write(STDOUT_FILENO, tip, strlen(tip));
	char school[100];
	memset(school, 0, sizeof(school));
	read(STDIN_FILENO, school, sizeof(school));
	school[strlen(school) - 1] = 0;

	sprintf(SQL, "INSERT INTO pal_people (name, sex, age, race, school) VALUES ('%s', '%s', '%s', '%s', '%s')"
	        , name, sex, age, race, school);

	//printf("%s\n", SQL);
	if (mysql_query(connection, SQL) != 0)
	{
		printf("Error in mysql_query:INSERT.\n%s\n", mysql_error(&mysql));
		return -1;
	}
	return 0;
}

int selectname()
{
	if (mysql_query(connection, "SELECT * FROM pal_people") != 0)
	{
		printf("Error in SELECT.\n%s\n", mysql_error(&mysql));
	}
	//MYSQL_RES结果集
	MYSQL_RES *result = mysql_store_result(connection);
	MYSQL_ROW row = mysql_fetch_row(result);
	while (row != NULL)
	{
		printf("%-20s%-4s%-4s%-14s%s\n", row[1], row[2], row[3], row[4], row[5]);
		row = mysql_fetch_row(result);
	}
	mysql_free_result(result);
	return 0;
}

int main(int argc, char const *argv[])
{
	//system("stty erase ^H");//不显示退格键回显

	mysql_init(&mysql);//初始化SOCKET和MYSQL数据结构
	connection = mysql_real_connect(&mysql, "192.168.1.112", "dbatest", "dbatest", "dbtest", 0, 0, 0);
	if (connection == NULL)
	{
		printf("Error in MySQL Connect.\n%s\n", mysql_error(&mysql));//调用MYSQL错误函数
		return -1;
	}
	if (mysql_query(connection, "SET NAMES utf8") != 0)
	{
		printf("Error in SET NAMES.\n%s\n", mysql_error(&mysql));
		return -1;
	}
	//deletename();
	//insertname();
	selectname();
	mysql_close(connection);//断开MYSQL链接
	printf("OK！\n");
	return 0;
}