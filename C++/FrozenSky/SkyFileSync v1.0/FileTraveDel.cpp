#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <io.h>
#include <direct.h>

using namespace std;

/*
[Function]
	遍历srcDir路径下的所有文件和文件夹，与destDir路径下的文件进行比对，删除srcDir路径下相对于destDir路径不存在的文件和文件夹。
	curPath用以记录相对路径。
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
		if (FileInfo.attrib & _A_SUBDIR) //当前文件是目录
		{
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0)) //忽略.和..目录
			{
				string tmpCurPath = string(curPath) + "\\" + string(FileInfo.name);
				string destDirPath = string(destDir) + tmpCurPath;
				fileTraveDel(srcDir, tmpCurPath.c_str(), destDir); //优先进入下级目录递归
				if (_access(destDirPath.c_str(), 0) == -1) //目标目录不存在
				{
					string delDirPath = string(srcDir) + tmpCurPath;
					cout << "★是否删除目录 " << delDirPath.c_str() << " ？ [Y/N]" << endl;
					char yorn;
					cin >> yorn;
					if (yorn == 'y' || yorn == 'Y')
					{
						if (_rmdir(delDirPath.c_str()) == -1)
						{
							cout << "◆删除目录 " << delDirPath.c_str() << "失败" << endl;
							_findclose(Handle);
							return -2;
						}
						else
						{
							cout << "★删除目录 " << delDirPath.c_str() << endl;
						}
					}
				}
			}
		}
		else //当前文件是文件（雾）
		{
			string tmpCurPath = string(curPath) + "\\" + string(FileInfo.name);
			string destFilePath = string(destDir) + tmpCurPath;
			if (_access(destFilePath.c_str(), 0) == -1)
			{
				string delFilePath = string(srcDir) + tmpCurPath;
				cout << "☆是否删除文件 " << delFilePath.c_str() << " ？ [Y/N]" << endl;
				char yorn;
				cin >> yorn;
				if (yorn == 'y' || yorn == 'Y')
				{
					if (remove(delFilePath.c_str()) == -1)
					{
						cout << "◆删除文件 " << delFilePath.c_str() << "失败" << endl;
						_findclose(Handle);
						return -3;
					}
					else
					{
						cout << "☆删除文件 " << delFilePath.c_str() << endl;
					}
				}
			}
		}
	} while (_findnext(Handle, &FileInfo) == 0);
	_findclose(Handle);
	return 0;
}