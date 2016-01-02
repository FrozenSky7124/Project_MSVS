MySQL
=====

```sql
	//创建数据库&设置字符集
	CREATE DATABASE dbtest CHARACTER SET utf8;

	//设置MySQL环境字符集
	SET NAMES utf8;

	//创建新用户
	GRANT ALL ON db_name.* TO user_name IDENTIFIED BY 'password';

	//创建表
	CREATE TABLE table1 (name varchar(100), sex varchar(10), age int);
	CREATE TABLE table2 (id int(10) NOT NULL auto_increment,
						name varchar(20), age int, PRIMARY KEY(id));

	//增
	INSERT INTO table1 (name, sex, age) VALUES ('name', 'man', '50');
	INSERT INTO table2 (name, age) VALUES ('name', '50');

	//查
	SELECT name, sex, age FROM table1;
	SELECT * FROM table1 LIMIT m,n;
	SELECT * FROM table1 WHERE age > 20;
```

**GROUP BY**

**ORDER BY**

**索引**

```sql
	//普通索引
	CREATE INDEX table1_index ON table1 (age);
	//唯一索引
	//PRIMARY KEY 主键将自动建立唯一索引
	CREATE UNIQUE INDEX table1_index ON table1 (age);
```