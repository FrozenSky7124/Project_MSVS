#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <io.h>
#include <direct.h>

using namespace std;

/*
[Function]
	����srcDir·���µ������ļ����ļ��У���destDir·���µ��ļ����бȶԣ�ɾ��srcDir·���������destDir·�������ڵ��ļ����ļ��С�
	curPath���Լ�¼���·����
[Return Value]
	0 : Success!
	-1: Error in _findfirst()
	-2: Error in _rmdir()
	-3: Error in remove()
*/
int fileTraveDel(const char *srcDir, const char *curPath, const char *destDir)
{
	_finddata_t FileInfo;
	string srcDir_p = string(srcDir) + string(curPath) + "\\*";
	long Handle = _findfirst(srcDir_p.c_str(), &FileInfo);
	if (Handle == -1)
	{
		cout << "Error: Can't Find SrcDir!" << endl;
		return -1;
	}
	do
	{
		if (FileInfo.attrib & _A_SUBDIR) //��ǰ�ļ���Ŀ¼
		{
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0)) //����.��..Ŀ¼
			{
				string tmpCurPath = string(curPath) + "\\" + string(FileInfo.name);
				string destDirPath = string(destDir) + tmpCurPath;
				fileTraveDel(srcDir, tmpCurPath.c_str(), destDir); //���Ƚ����¼�Ŀ¼�ݹ�
				if (_access(destDirPath.c_str(), 0) == -1) //Ŀ��Ŀ¼������
				{
					string delDirPath = string(srcDir) + tmpCurPath;
					cout << "���Ƿ�ɾ��Ŀ¼ " << delDirPath.c_str() << " �� [Y/N]" << endl;
					char yorn;
					cin >> yorn;
					if (yorn == 'y' || yorn == 'Y')
					{
						if (_rmdir(delDirPath.c_str()) == -1)
						{
							cout << "��ɾ��Ŀ¼ " << delDirPath.c_str() << "ʧ��" << endl;
							_findclose(Handle);
							return -2;
						}
						else
						{
							cout << "��ɾ��Ŀ¼ " << delDirPath.c_str() << endl;
						}
					}
				}
			}
		}
		else //��ǰ�ļ����ļ�����
		{
			string tmpCurPath = string(curPath) + "\\" + string(FileInfo.name);
			string destFilePath = string(destDir) + tmpCurPath;
			if (_access(destFilePath.c_str(), 0) == -1)
			{
				string delFilePath = string(srcDir) + tmpCurPath;
				cout << "���Ƿ�ɾ���ļ� " << delFilePath.c_str() << " �� [Y/N]" << endl;
				char yorn;
				cin >> yorn;
				if (yorn == 'y' || yorn == 'Y')
				{
					if (remove(delFilePath.c_str()) == -1)
					{
						cout << "��ɾ���ļ� " << delFilePath.c_str() << "ʧ��" << endl;
						_findclose(Handle);
						return -3;
					}
					else
					{
						cout << "��ɾ���ļ� " << delFilePath.c_str() << endl;
					}
				}
			}
		}
	} while (_findnext(Handle, &FileInfo) == 0);
	_findclose(Handle);
	return 0;
}