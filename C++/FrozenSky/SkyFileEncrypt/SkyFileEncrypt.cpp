#define _CRT_SECURE_NO_WARNINGS
#define BUF_SIZE sizeof(int)
#include <iostream>

using namespace std;

int fileEncrypt(char* filePath, unsigned int secretKey)
{
	FILE* file = fopen(filePath, "rb+");
	if (!file)
	{
		cerr << "Unable to open file: " << filePath << endl;
		return -1;
	}

	void* buffer = NULL;
	buffer = malloc(sizeof(char));
	if (!buffer)
	{
		cout << "Unable to malloc buffer!" << endl;
		return -2;
	}
	memset(buffer, 0, sizeof(char));

	// Get fileSize
	struct _stat info;
	_stat(filePath, &info);
	int fileSize = info.st_size;
	cout << "File Size: " << fileSize << endl;

	if (fileSize == 0)
	{
		free(buffer);
		fclose(file);
		return -4;
	}

	int operateNum = 100;
	if (fileSize < 100)
	{
		operateNum = fileSize;
	}

	for (int i = 0; i < operateNum; i++)
	{
		fseek(file, i, SEEK_SET);
		if (fread(buffer, sizeof(char), 1, file) != 1)
		{
			cout << "Unable to read Data!" << endl;
			return -3;
		}
		else
		{
			printf("%02X ", (*(unsigned char*)buffer));
			unsigned char cEncrypt = (unsigned char)((*(unsigned char*)buffer) ^ secretKey);
			//printf("%02X ", (unsigned char)((*(unsigned char*)buffer) ^ secretKey));
			//printf("%02X ", cEncrypt);
			
			fseek(file, i, SEEK_SET);
			fwrite(&cEncrypt, sizeof(char), 1, file);
			//fseek(file, 1, SEEK_CUR);
			memset(buffer, 0, sizeof(char));
		}
	}

	free(buffer);
	fclose(file);
	return 0;
}

int main(int argc, char* argv[])
{
	// Paramter1: FilePath
	// Paramter2: Key
	if (argc != 3)
	{
		cout << "Parameters Error !" << endl;
		system("pause");
		return -1;
	}
	unsigned int secretKey = atoi(argv[2]);
	cout << argv[0] << endl;
	cout << argv[1] << endl;
	cout << secretKey << endl;

	int ret = fileEncrypt(argv[1], secretKey);
	if (ret != 0)
	{
		cout << endl << "Error in Function: fileEncrypt!" << endl;
	}
	else
	{
		cout << endl << "File Encrypt Complete!" << endl;
	}
	
	system("pause");
	return 0;
}