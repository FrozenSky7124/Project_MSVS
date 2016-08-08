#include <iostream>
#include <fstream>
#include "FileCopy.h"

using namespace std;

bool copyFile(const char *srcFile, const char *destFile)
{
	ifstream fin;
	ofstream fout;

	fin.open(srcFile, ios::binary | ios::in); //二进制只读模式打开文件
	if (fin.fail()) //源文件打开失败
	{
		cout << "Error: Can't Open SrcFile: " << srcFile << endl;
		fin.close();
		fout.close();
		return false;
	}
	fout.open(destFile, ios::binary | ios::trunc); //二进制截断模式打开文件
	if (fout.fail()) //目标文件打开失败
	{
		cout << "Error: Can't Open DestFile: " << destFile << endl;
		fin.close();
		fout.close();
		return false;
	}

	fout << fin.rdbuf();
	fin.close();
	fout.close();
	return true;
}