#define _CRT_SECURE_NO_WARNINGS

#include "FileTrave.h"
#include "FileCopy.h"
#include <iostream>
#include <io.h>
#include <direct.h>

using namespace std;

/*
[Function]
	遍历srcDir路径下的所有文件和文件夹，与destDir路径下的文件进行比对，更新destDir路径下对应的文件。
	curPath用以记录相对路径。
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
		//判断是否是目录
		if (FileInfo.attrib & _A_SUBDIR)
		{
			//判断是否是上级目录链接
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				string tmpCurPath = string(curPath) + "\\" + string(FileInfo.name);
				//判断是否需要更新目录
				string destDirFile = string(destDir) + tmpCurPath;
				if (_access(destDirFile.c_str(), 6) == -1) //目标目录不存在
				{
					if (_mkdir(destDirFile.c_str()) == -1) //创建目录
					{
						cout << "Error: _mkdir() fail!" << endl;
						_findclose(Handle);
						return -3;
					}
					else
					{
						cout << "★创建目录 " << tmpCurPath.c_str() << endl;
					}
				}
				//cout << "目录 " << tmpCurPath.c_str() << endl;
				fileTrave(srcDir, tmpCurPath.c_str(), destDir);
			}
		}
		else
		{
			string tmpCurPath = string(curPath) + "\\" + string(FileInfo.name);
			//判断是否需要更新文件
			string destFile = string(destDir) + tmpCurPath;
			_finddata_t destFileInfo;
			long Handle_DestFile = _findfirst(destFile.c_str(), &destFileInfo);
			if (Handle_DestFile == -1) //目标文件不存在
			{
				string srcFile = string(srcDir) + tmpCurPath;
				if (copyFile(srcFile.c_str(), destFile.c_str()))
					cout << "☆复制文件 " << tmpCurPath.c_str() << endl;
				else
				{
					cout << "◆复制文件 " << tmpCurPath.c_str() << "失败" << endl;
					_findclose(Handle_DestFile);
					_findclose(Handle);
					return -2;
				}
			}
			else
			{
				if (FileInfo.time_write >= destFileInfo.time_write) //判断文件修改时间是否相同
				{
					string srcFile = string(srcDir) + tmpCurPath;
					if (copyFile(srcFile.c_str(), destFile.c_str()))
						cout << "☆复制文件 " << tmpCurPath.c_str() << endl;
					else
					{
						cout << "◆复制文件 " << tmpCurPath.c_str() << "失败" << endl;
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