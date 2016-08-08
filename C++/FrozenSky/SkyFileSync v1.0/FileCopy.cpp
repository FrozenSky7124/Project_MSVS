#include <iostream>
#include <fstream>
#include "FileCopy.h"

using namespace std;

bool copyFile(const char *srcFile, const char *destFile)
{
	ifstream fin;
	ofstream fout;

	fin.open(srcFile, ios::binary | ios::in); //������ֻ��ģʽ���ļ�
	if (fin.fail()) //Դ�ļ���ʧ��
	{
		cout << "Error: Can't Open SrcFile: " << srcFile << endl;
		fin.close();
		fout.close();
		return false;
	}
	fout.open(destFile, ios::binary | ios::trunc); //�����ƽض�ģʽ���ļ�
	if (fout.fail()) //Ŀ���ļ���ʧ��
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