/*
 * mydb.h
 *
 *  Created on: 2013年12月27日
 *      Author: zhujy
 */
#ifndef MYDB_H_
#define MYDB_H_


void init_db();
int conn_db(const char *hostname, const char *username, const char *password,
		const char *dbname);
void disconn_db();
int open_db(const char *SQL);
int exec_db(const char *SQL);

#endif /* MYDB_H_ */
