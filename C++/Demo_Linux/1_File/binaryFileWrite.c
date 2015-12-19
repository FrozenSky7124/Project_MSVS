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
		struct person man[10];
		memset(&man, 0, sizeof(man));

		man[0].id = 0;
		strcpy(man[0].name, "A1");
		man[0].age = 50;
		man[0].sex = 1;
		strcpy(man[0].tel, "123");

		man[1].id = 1;
		strcpy(man[1].name, "A2");
		man[1].age = 20;
		man[1].sex = 0;
		strcpy(man[1].tel, "456");

		fwrite(&man, sizeof(struct person), 2, p);
		fclose(p);
	}
	return 0;
}