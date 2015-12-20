#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char const *argv[])
{
	char *myLogin = getlogin();
	struct passwd *myPs = getpwnam(myLogin);
	printf("UserName:%s\n", myPs->pw_name );
	printf("UID:%d\n", myPs->pw_uid);
	printf("HomeDir:%s\n", myPs->pw_dir);
	return 0;
}