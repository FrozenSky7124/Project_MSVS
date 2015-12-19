struct person
{
	int id;
	char name[20];
	int age;
	int sex;
	char tel[20];
};

int main(int arg, char *args[])
{
	FILE *p = fopen(args[1], "w");
	if (p == NULL)
	{
		printf("error is %s\n", strerror(errno));
	} else
	{
		printf("success\n");
		struct person man;
		memset(&man, 0, sizeof(man));

		while(fread(&man, sizeof(struct person), 1, p))
		{
			printf("id=%d\n", man.id);
			printf("name=%s\n", man.name);
			printf("age=%d\n", man.age);
			printf("tel=%s\n", man.tel);
		}
		fclose(p);
	}
	return 0;
}