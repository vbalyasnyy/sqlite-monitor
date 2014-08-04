/* LOG TYPE */
//#define MON_LOG_TYPE FILE			/* FILE or DAEMON */
#define MON_LOG_FILE_NAME "/tmp/sqlite.log"	/* by default */

/* SQLITE_OBJECT */
#define MON_OBJ_CON ON				/* DB connections */
#define MON_OBJ_QUE ON				/* SQL queries */

/* VERBOSE LEVEL */
#define MON_VERBOSE_LEVEL_1 ON			/* 1. Just event message */
						/* 2. Time of instruction */

#include "time.h"

/**********************************************/
/* LOGGER */
/**********************************************/
int
file_log(const char* fmt, ...){
        va_list ap;
        va_start(ap, fmt);

        FILE *fp;
#ifdef MON_LOG_FILE_NAME
        fp = fopen(MON_LOG_FILE_NAME, "a+");
#else
        fp = fopen("/tmp/sqlilte.log", "a+");
#endif
        if (fp != NULL) {
                fprintf(fp, "%5d            ", getpid());
                vfprintf(fp, fmt, ap);
                fprintf(fp, "\n");
                fclose(fp);
        }

        va_end(ap);
        return 0;
}

int
file_log_t(const char* fmt, ...){
	static char color = 0;
	static struct timeval prev_time;
	static char init_time = 0;
	if(!init_time) {
		gettimeofday(&prev_time, NULL);
		init_time = 1;
	}

	struct timeval now;
	gettimeofday(&now, NULL);

        va_list ap;
        va_start(ap, fmt);

        FILE *fp;
#ifdef MON_LOG_FILE_NAME
        fp = fopen(MON_LOG_FILE_NAME, "a+");
#else
        fp = fopen("/tmp/sqlilte.log", "a+");
#endif
        if (fp != NULL) {
		if(color) {
			fprintf(fp, "\x1b[32m");
			color = 0;
		} else {
			fprintf(fp, "\x1b[37m");
			color = 1;
		}
                fprintf(fp, "%5d %3ld %6ld ", getpid(), (prev_time.tv_sec - now.tv_sec), (now.tv_usec - prev_time.tv_usec));
                vfprintf(fp, fmt, ap);
                fprintf(fp, "\n");
                fclose(fp);
        }

        va_end(ap);
	prev_time = now;
        return 0;
}
#define MON_MSG(fmt, arg...) file_log(fmt,##arg)
#define MON_MSG_T(fmt, arg...) file_log_t(fmt,##arg)

/**********************************************/
/* SQLITE OBJECTS */
/**********************************************/

void sqlite_obj_db_print(struct sqlite3 *db) {
	int i;
	/* OS interface */
	if(db->pVfs) {
		sqlite3_vfs *V = db->pVfs;
		MON_MSG("->OS interfaces:");
		do {
			MON_MSG("->	%s", V->zName);
			V = V->pNext;
		} while(V);
	}
	/* backends */
	for(i=0; i<db->nDb; i++) {
		MON_MSG("->backend number %d", i);
		MON_MSG("->	backend name - %s", db->aDb[i].zName);
		/* Btree */
/*		MON_MSG("	Btree");
		struct Btree* V = db->aDb[i].pBt;
		MON_MSG("          %x", V);
*/		/* Schema */
/*		MON_MSG("	SCHEMA (COO %d, GEN %d, SIZE %d)", db->aDb[i].pSchema->schema_cookie, db->aDb[i].pSchema->iGeneration, db->aDb[i].pSchema->cache_size);
*/	}
}

void sqlite_obj_db_open_start(const char *zFilename) {
	MON_MSG_T("*** open %s database ***", zFilename);
}

void sqlite_obj_db_open_stop(struct sqlite3 *db) {
	sqlite_obj_db_print(db);
	MON_MSG_T("*** finish open database ***");
}

void sqlite_obj_db_close_start(struct sqlite3 *db) {
	MON_MSG_T("*** close %s database ***", sqlite3BtreeGetFilename(db->aDb[0].pBt));
}

void sqlite_obj_db_close_stop(void) {
	MON_MSG_T("*** finish close database ***");
}

void sqlite_obj_qe_start(const char *zSql) {
        MON_MSG_T("*** query %s ***", zSql);
}

void sqlite_obj_qe_stop(void) {
        MON_MSG_T("*** finish query ***");
}

