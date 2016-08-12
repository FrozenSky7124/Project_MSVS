#define _CRT_SECURE_NO_WARNINGS

#include "FileTrave.h"
#include "FileCopy.h"
#include <iostream>
#include <io.h>
#include <direct.h>

using namespace std;

/*
[Function]
	����srcDir·���µ������ļ����ļ��У���destDir·���µ��ļ����бȶԣ�����destDir·���¶�Ӧ���ļ���
	curPath���Լ�¼���·����
[Return Value]
	0 : Success!
	-1: Error in _findfirst()
	-2: Error in copyFile()
	-3: Error in _mkdir()
*/
int fileTrave(const char *srcDir, const char *curPath, const char *destDir)
{
	_finddata_t FileInfo;
	string srcDir_p = string(srcDir) + string(curPath) + "\\*";
	long Handle = _findfirst(srcDir_p.c_str(), &FileInfo);
	if (Handle == -1L)
	{
		cout << "Error: Can't Find SrcDir!" << endl;
		return -1;
	}

	do
	{
		//�ж��Ƿ���Ŀ¼
		if (FileInfo.attrib & _A_SUBDIR)
		{
			//�ж��Ƿ����ϼ�Ŀ¼����
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				string tmpCurPath = string(curPath) + "\\" + string(FileInfo.name);
				//�ж��Ƿ���Ҫ����Ŀ¼
				string destDirFile = string(destDir) + tmpCurPath;
				if (_access(destDirFile.c_str(), 6) == -1) //Ŀ��Ŀ¼������
				{
					if (_mkdir(destDirFile.c_str()) == -1) //����Ŀ¼
					{
						cout << "Error: _mkdir() fail!" << endl;
						_findclose(Handle);
						return -3;
					}
					else
					{
						cout << "�ﴴ��Ŀ¼ " << tmpCurPath.c_str() << endl;
					}
				}
				//cout << "Ŀ¼ " << tmpCurPath.c_str() << endl;
				fileTrave(srcDir, tmpCurPath.c_str(), destDir);
			}
		}
		else
		{
			string tmpCurPath = string(curPath) + "\\" + string(FileInfo.name);
			//�ж��Ƿ���Ҫ�����ļ�
			string destFile = string(destDir) + tmpCurPath;
			_finddata_t destFileInfo;
			long Handle_DestFile = _findfirst(destFile.c_str(), &destFileInfo);
			if (Handle_DestFile == -1) //Ŀ���ļ�������
			{
				string srcFile = string(srcDir) + tmpCurPath;
				if (copyFile(srcFile.c_str(), destFile.c_str()))
					cout << "����ļ� " << tmpCurPath.c_str() << endl;
				else
				{
					cout << "�������ļ� " << tmpCurPath.c_str() << "ʧ��" << endl;
					_findclose(Handle_DestFile);
					_findclose(Handle);
					return -2;
				}
			}
			else
			{
				if (FileInfo.time_write >= destFileInfo.time_write) //�ж��ļ��޸�ʱ���Ƿ���ͬ
				{
					string srcFile = string(srcDir) + tmpCurPath;
					if (copyFile(srcFile.c_str(), destFile.c_str()))
						cout << "����ļ� " << tmpCurPath.c_str() << endl;
					else
					{
						cout << "�������ļ� " << tmpCurPath.c_str() << "ʧ��" << endl;
						_findclose(Handle_DestFile);
						_findclose(Handle);
						return -2;
					}
				}
			}
			_findclose(Handle_DestFile);
			//cout << tmpCurPath.c_str() << "[Handle]" << Handle_DestFile << endl;
		}
	} while (_findnext(Handle, &FileInfo) == 0);
	_findclose(Handle);
	return 0;
}