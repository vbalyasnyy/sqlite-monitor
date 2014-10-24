#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include <stdlib.h>

char * sql = "select field1,field2,field3 from test";
char* dbname;

sqlite3 * db;
sqlite3_stmt *stmt;

int date;
char *description;
char *venue;

int main(int argc, char* argv[])
{
	if( argc<0 ) {
		printf("Usage: ./%s dbname\n", argv[0]);
		return 1;
	} else
		dbname = argv[1];

	/* open the database */
	int result=sqlite3_open(dbname,&db) ;
	if (result != SQLITE_OK) {
		printf("Failed to open database %s\n\r",sqlite3_errstr(result)) ;
		sqlite3_close(db) ;
		return 1;
	}

	/* prepare the sql, leave stmt ready for loop */
	result = sqlite3_prepare_v2(db, sql, strlen(sql)+1, &stmt, NULL) ;
	if (result != SQLITE_OK) {
		printf("Failed to prepare database %s\n\r",sqlite3_errstr(result)) ;
		sqlite3_close(db) ;
		return 1;
        }

	/* allocate memory for decsription and venue */
	description = (char *)malloc(100) ;
	venue = (char *)malloc(100) ;

	/* loop reading each row until step returns anything other than SQLITE_ROW */
	do {
		result = sqlite3_step (stmt) ;
		if (result == SQLITE_ROW) { /* can read data */
			date = sqlite3_column_int(stmt,0) ;
			strcpy(description, (char *)sqlite3_column_text(stmt,1)) ;
			strcpy(venue, (char *)sqlite3_column_text(stmt,2)) ;
		}
	} while (result == SQLITE_ROW) ;

	sqlite3_close(db) ;
	free(description) ;
	free(venue) ;
	return 0;
}

