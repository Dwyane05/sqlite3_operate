/*
 * test1.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: cui
 */

//example	http://zetcode.com/db/sqlitec/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sqlite3.h>

using namespace std;

int callback(void *, int, char **, char **);

void print_version(sqlite3 *db, sqlite3_stmt *stmt )
{
	int rc;
	rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
		return;
	}

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		printf("%s\n", sqlite3_column_text(stmt, 0));
	}
	sqlite3_finalize(stmt);

	//sqlite3_libversion() get sqlite3 version
//	printf("%s\n", sqlite3_libversion());
}

void create_table(int &rc, sqlite3 *db, char *sql, sqlite3_stmt *stmt)
{
	rc = sqlite3_prepare_v2( db, sql, strlen(sql), &stmt, 0 );
	if(rc){
		 fprintf(stderr,"object convert error: %s\n",sqlite3_errmsg(db));
		 return;
	}
	//exec sentence
	sqlite3_step(stmt);
	//free memory
	sqlite3_finalize(stmt);
}

void insert_strip( int &rc, char *sql, sqlite3 *db, sqlite3_stmt *stmt, const char name[] )
{
	sprintf( sql, "INSERT INTO MyTable VALUES(NULL, '%s');", name);
	rc=sqlite3_prepare_v2( db, sql, strlen(sql), &stmt, 0 );
	if(rc){
		fprintf(stderr,"object convert error: %s\n",sqlite3_errmsg(db));
		return;
	}
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

bool create_car_table(sqlite3 *db)
{
	char *err_msg = 0;
	char sql[] = "DROP TABLE IF EXISTS Cars;"
				"CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
				"INSERT INTO Cars VALUES(1, 'Audi', 52642);"
				"INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
				"INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
				"INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
				"INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
				"INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
				"INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
				"INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if (rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		return false;
	}
	return true;
}

void parameter_query( sqlite3 *db, sqlite3_stmt *stmt, int value )
{
	char *err_msg = 0;
	const char *sql = "SELECT Id, Name FROM Cars WHERE Id = ?";

	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	if (rc == SQLITE_OK) {
	 sqlite3_bind_int(stmt, 1, value);
	} else {
	 fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	int step = sqlite3_step(stmt);
	if (step == SQLITE_ROW) {
	 printf("%s: ", sqlite3_column_text(stmt, 0));
	 printf("%s\n", sqlite3_column_text(stmt, 1));
	}
	sqlite3_finalize(stmt);
}

void cars_id_name(sqlite3 *db, sqlite3_stmt *stmt, int value)
{
	char *err_msg = 0;
	const char *sql = "SELECT Id, Name FROM Cars WHERE Id = @id";

	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc == SQLITE_OK) {
		int idx = sqlite3_bind_parameter_index(stmt, "@id");
//		int value = 4;
		sqlite3_bind_int(stmt, idx, value);
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	int step = sqlite3_step(stmt);
	if (step == SQLITE_ROW) {
		printf("%s: ", sqlite3_column_text(stmt, 0));
		printf("%s\n", sqlite3_column_text(stmt, 1));
	}

	sqlite3_finalize(stmt);
}

void hexadecimal()
{

	int bin = 0b10;
	int obc = 010;
	int dex = 10;
	int hex = 0x10;

	cout << "bin: " << bin << endl;
	cout << "obc: " << obc << endl;
	cout << "dex: " << dex << endl;
	cout << "hex: " << hex << endl;

}

void insert_image(sqlite3 *db, sqlite3_stmt *pStmt)
{
	FILE *fp = fopen( "women.jpg", "rb" );
	if(NULL == fp ){
		 fprintf(stderr, "Cannot open image file\n");
		 return;
	}
	fseek( fp, 0, SEEK_END );
	if( ferror(fp) ){
		fprintf(stderr, "fseek() failed\n");
		int r = fclose(fp);
		if (r == EOF) {
			fprintf(stderr, "Cannot close file handler\n");
		}
		return;
	}

	int flen = ftell(fp);
	if(-1 == flen){
		perror("error occurred");
		int r = fclose(fp);
		if (r == EOF) {
			fprintf(stderr, "Cannot close file handler\n");
		}
		return;
	}

	fseek(fp, 0, SEEK_SET);
	if(ferror(fp)){
		fprintf(stderr, "fseek() failed\n");
		int r = fclose(fp);
		if (r == EOF) {
			fprintf(stderr, "Cannot close file handler\n");
		}
		return;
	}

	 char data[flen+1];
	 int size = fread(data, 1, flen, fp);
	 if (ferror(fp)) {
		 fprintf(stderr, "fread() failed\n");
		 int r = fclose(fp);
		 if (r == EOF) {
			 fprintf(stderr, "Cannot close file handler\n");
		 }
		 return;
	}

	 int r = fclose(fp);
	 if (r == EOF) {
		 fprintf(stderr, "Cannot close file handler\n");
	 }

	 char *err_msg = 0;
	 char sql[] = "INSERT INTO Images(Data) VALUES(?)";
	 int  rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
	 if (rc != SQLITE_OK) {
		 fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
		 return;
	 }

	 sqlite3_bind_blob(pStmt, 1, data, size, SQLITE_STATIC);
	 rc = sqlite3_step(pStmt);
	 if (rc != SQLITE_DONE) {
		 printf("execution failed: %s", sqlite3_errmsg(db));
	 }
	 sqlite3_finalize(pStmt);

}

void create_image_table(sqlite3 *db, sqlite3_stmt *stmt)
{
	char sql[] = "CREATE TABLE IF NOT EXISTS Images(Id INTEGER PRIMARY KEY, Data BLOB);";
	int rc = sqlite3_prepare_v2( db, sql, strlen(sql), &stmt, 0 );
	if(rc){
		 fprintf(stderr,"object convert error: %s\n",sqlite3_errmsg(db));
		 return;
	}
	//exec sentence
	sqlite3_step(stmt);
	//free memory
	sqlite3_finalize(stmt);
}

void read_image_from_db(sqlite3 *db, sqlite3_stmt *pStmt)
{
	FILE *fp = fopen( "women2.jpg", "wb");
	if (NULL == fp){
		fprintf(stderr, "Cannot open image file\n");
		return;
	}
	char *err_msg = 0;
	const char *sql = "SELECT Data FROM Images WHERE Id = 1";
	int rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
	if (SQLITE_OK != rc){
		fprintf(stderr, "Failed to prepare statement\n");
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
//		sqlite3_close(db);
		return;
	}
	rc = sqlite3_step(pStmt);
	int bytes = 0;
	if (rc == SQLITE_ROW) {
		bytes = sqlite3_column_bytes(pStmt, 0);
	}

	fwrite(sqlite3_column_blob(pStmt, 0), bytes, 1, fp);
	if (ferror(fp)) {
		fprintf(stderr, "fwrite() failed\n");
		return;
	}

	int r = fclose(fp);
	if (r == EOF) {
		fprintf(stderr, "Cannot close file handler\n");
	}

	sqlite3_finalize(pStmt);
}

void table_metadata(sqlite3 *db)
{
	char *err_msg = 0;
//	const char *sql = "PRAGMA table_info(Cars)";
	const char *sql = "SELECT name FROM sqlite_master WHERE type='table'";

	int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
	if (rc != SQLITE_OK ) {

		fprintf(stderr, "Failed to select data\n");
		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);
		return;
	}
}

void transaction(sqlite3 *db)
{
	char *err_msg = 0;
	const char *sql = "DROP TABLE IF EXISTS Friends;"
            "BEGIN TRANSACTION;"
            "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);"
            "INSERT INTO Friends(Name) VALUES ('Tom');"
            "INSERT INTO Friends(Name) VALUES ('Rebecca');"
            "INSERT INTO Friends(Name) VALUES ('Jim');"
            "INSERT INTO Friend(Name) VALUES ('Robert');"
            "COMMIT;";

	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if (rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);

		return;
	}

}

int main( int argc,  char *argv[] )
{
	sqlite3 *db;
	sqlite3_stmt *stmt = 0;


	//create table structure, key autoincrement
	char sql[]= "CREATE TABLE IF NOT EXISTS MyTable(ID integer NOT NULL primary "
		"key autoincrement,Name nvarchar(32));";

	int rc = sqlite3_open( "MyDB.db", &db );
	if(rc != SQLITE_OK){
		sqlite3_close(db);
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	//log version
	print_version( db, stmt);
	printf("Autocommit: %d\n", sqlite3_get_autocommit(db));
	transaction(db);

	create_table(rc, db, sql, stmt );
	create_image_table(db, stmt);
	create_car_table(db);
	insert_strip( rc, sql, db, stmt, "zhangsan" );

	int last_id = sqlite3_last_insert_rowid(db);
	printf("The last Id of the inserted row is %d\n", last_id);

	sprintf(sql,"SELECT * FROM Cars;");
//	sprintf(sql,"SELECT * FROM Images;");
	char *err_msg = 0;
	rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n",
                sqlite3_errmsg(db));
    }

	sprintf(sql,"SELECT * FROM MyTable;");
	sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,0);
	int id;
	unsigned char *name;
	 //[3]循环执行stmt对象,读取数据库里边的数据
	while(sqlite3_step(stmt)==SQLITE_ROW)  {
		//[4]绑定变量
		id=sqlite3_column_int(stmt,0);
		name=(unsigned char *)sqlite3_column_text(stmt,1);
		cout<<id<<'\t'<<name<<endl;
	}

	parameter_query(db, stmt, 3);
	cars_id_name(db, stmt, 4);
	sqlite3_finalize(stmt);

//	insert_image(db, stmt);
	read_image_from_db(db, stmt);
	table_metadata(db);

	sqlite3_close(db);
	hexadecimal();


	return 0;
}

int callback(void *NotUsed, int argc, char **argv,
                    char **azColName) {

    NotUsed = 0;

    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}
