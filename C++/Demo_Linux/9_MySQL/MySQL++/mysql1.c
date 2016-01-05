/*
 * data.c
 *
 *  Created on: 2013年12月27日
 *      Author: zhujy
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <termios.h>
#include "mydb.h"


void sqldb(const char *src)//参数src为要执行的SQL语句
{
	if ((strncmp(src, "select", 6) == 0) || (strncmp(src, "SELECT", 6) == 0)
			|| (strncmp(src, "show", 4) == 0) || (strncmp(src, "SHOW", 4) == 0)
			|| (strncmp(src, "desc", 4) == 0) || (strncmp(src, "DESC", 4) == 0))
	{
		open_db(src);//如果src为有返回数据集SQL语句，那么调用open_db函数
	} else
	{
		exec_db(src);//如果src为没有有返回数据集SQL语句，那么调用exec_db函数
	}
}



void work(const char *userid, const char *password)
{
	init_db();
	if (conn_db("localhost", userid, password, "test") != 0)//连接到数据库
	{
		return;//连接数据库失败，函数退出
	}
	char buf[2048];
	while (1)//循环从键盘读取
	{
		write(STDOUT_FILENO, "mysql1>", strlen("mysql1>"));//屏幕输出命令提示符mysql1>
		memset(buf, 0, sizeof(buf));
		read(STDIN_FILENO, buf, sizeof(buf));//等待用户从键盘输入
		if (strncmp(buf, "quit", 4) == 0)
			break;//用户输入quit,循环break;
		sqldb(buf);
	}
	disconn_db();//断开数据库连接
}


struct termios oldterm;
void setstty()//设置输入退格键，不回显
{
	//system("stty erase ^H");//执行shell命令，也可以 用来设置读取用户键盘输入的时候，退格键不回显
	struct termios term;
	if(tcgetattr(STDIN_FILENO, &term) == -1)//得到系统termion的设置
	{
		printf("tcgetattr error is %s\n", strerror(errno));
		return;
	}

	oldterm = term;//保留当前termios设置，以便程序退出的时候可以恢复termios

	/*
	term.c_lflag &= ~ICANON;//取消ICANON选项（不规范输入）
	term.c_lflag |= ICANON;//设置ICANON选项(规范输入)
	term.c_cc字段为要设置的具体特殊输入字符，如c_cc[VERASE]代表退格键,
	term.c_cc[VERASE] = '\b';意思为把退格键修改为'\b'
	VERASE代表向前擦出一个字符,VINTR代表发送ctrl + C中断信号，ctrl + c的ASCII码为3
	例如：term.c_cc[VINTR] = '\t';意思为将tab键设置为终端信号
	tcsetattr中，第二个参数说明，TCSAFLUSH：发送了所有输出后更改才生效，在更改发生时，未读取的所有输入数据都被删除
	TCSANOW:更改立即生效
	TCSADRAIN:发送了所有输出后更改才发生，如果更改输出参数则应该使用该选项
	*/
	term.c_cc[VERASE] = '\b';//'\b'为退格键的ASCII码
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)//设置系统termion
	{
		printf("tcsetattr error is %s\n", strerror(errno));
	}
	return;
}

void returnstty()//恢复系统的termios设置
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm) == -1)//设置系统termion
	{
		printf("tcsetattr error is %s\n", strerror(errno));
	}
	return;
}


int main(int arg, char *args[])
{
	if (arg < 4)//如果没有参数，main函数退出
	{
		return EXIT_FAILURE;
	}

	if (strncmp(args[1], "-u", 2) != 0)//如果第二个参数不是-u，main函数退出
	{
		return EXIT_FAILURE;
	}

	if (strncmp(args[3], "-p", 2) != 0)//如果第四个参数不是-p，main函数退出
	{
		return EXIT_FAILURE;
	}

	const char *passwd = getpass("please input password:");//输入密码，屏幕不回显

	setstty();//设置输入退格键，不回显
	work(args[2], passwd);
	returnstty();//恢复系统的termios设置
	return EXIT_SUCCESS;
}

