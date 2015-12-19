int main(int arg, char *args[])
{
	if (arg <2)
		return 0;

	DIR *dp;
	struct dirent *dirp;
	dp = opendir(args[1]);//打开目录文件
	if (dp == NULL)
	{
		printf("error is %s\n", strerror(errno));
		return 0;
	}

	while((dirp = readdir(dp)) != NULL)//用readdir循环读取目录内容，读到目录尾，循环break
	{
		printf("%s\n", dirp->d_name);//将目录下的文件名打印到屏幕

	}

	closedir(dp);//关闭目录

	return 0;
}