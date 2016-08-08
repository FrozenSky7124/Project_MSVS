#include <iostream>
#include <direct.h>
#include "FileCopy.h"
#include "FileTrave.h"

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
		cout << "srcDir:  " << srcDir << endl;
		cout << "destDir: " << argv[1] << endl;
		fileTrave(srcDir, "", argv[1]);
	}
	else if (argc == 3)
	{
		cout << "Copying..." << endl;
		if (copyFile(argv[1], argv[2]))
		{
			cout << "Copy Success!" << endl;
		}
		else
		{
			cout << "Copy Fail!" << endl;
		}
	}
	return 0;
}