#include <iostream>
#include <direct.h>
#include "FileCopy.h"
#include "FileTrave.h"
#include "FileTraveDel.h"

using namespace std;

int main(int argc, char* argv[])
{
	char srcDir[256];

	if (argc < 2 || argc > 3)
	{
		cout << "Error: Parameters Wrong!" << endl;
		return 1;
	}
	else if (argc == 2)
	{
		/*
		[Function]
			char *_getcwd(char *buffer, int maxlen);
		[Parameters]
			buffer:	Storage location for the path.
			maxlen: Maximum length of the path in characters: char for _getcwd and wchar_t for _wgetcwd.
		[Return Value]
			Returns a pointer to buffer. A NULL return value indicates an error.
		*/
		_getcwd(srcDir, 256);
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		cout << "源 目 录: " << srcDir << endl;
		cout << "同步目录: " << argv[1] << endl;
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		int fTFunc = fileTrave(srcDir, "", argv[1]);
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		int fTDFunc = fileTraveDel(argv[1], "", srcDir);
		if (fTFunc == 0 && fTDFunc == 0)
		{
			cout << "                         >>>>>>>    同步完成！    <<<<<<<" << endl;
			cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		}
		else
		{
			cout << "                         >>>>>>>    同步失败！    <<<<<<<" << endl;
			cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
		}
	}
	else if (argc >= 3)
	{
		cout << ">>>>>>>     隐藏功能已禁用     <<<<<<<" << endl;
		//cout << "Copying..." << endl;
		//if (copyFile(argv[1], argv[2]))
		//{
		//	cout << "Copy Success!" << endl;
		//}
		//else
		//{
		//	cout << "Copy Fail!" << endl;
		//}
	}
	return 0;
}