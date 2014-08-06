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
/* COMMON FN */
/**********************************************/
/* http://stackoverflow.com/a/22214304 */
const char*
get_process_name_by_pid(const int pid)
{
    char* name = (char*)calloc(1024,sizeof(char));
    if(name){
        sprintf(name, "/proc/%d/cmdline",pid);
        FILE* f = fopen(name,"r");
        if(f){
            size_t size;
            size = fread(name, sizeof(char), 1024, f);
            if(size>0){
                if('\n'==name[size-1])
                    name[size-1]='\0';
            }
            fclose(f);
        }
    }
    return name;
}

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
                fprintf(fp, "%5d          * ", getpid());
                vfprintf(fp, fmt, ap);
                fprintf(fp, "\n");
                fclose(fp);
        }

        va_end(ap);
        return 0;
}

int
file_log_t(const char* fmt, ...){
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
		double diff = (now.tv_sec * 1000000 + now.tv_usec) - (prev_time.tv_sec * 1000000 + prev_time.tv_usec);
                fprintf(fp, "\x1b[32m%5d %10.6f\x1b[0m ", getpid(), diff/1000000);
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

void
sqlite_obj_db_print(struct sqlite3 *db) {
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

void
sqlite_obj_db_open_start(const char *zFilename) {
	const int pid = getpid();
	char *name = get_process_name_by_pid(pid);
	MON_MSG_T("PROCESS %s", name);
	MON_MSG_T("DB open start %s", zFilename);
	if(name)
		free(name);
}

void
sqlite_obj_db_open_stop(struct sqlite3 *db) {
	sqlite_obj_db_print(db);
	MON_MSG_T("DB open stop");
}

void
sqlite_obj_db_close_start(struct sqlite3 *db) {
	MON_MSG_T("DB close start %s", sqlite3BtreeGetFilename(db->aDb[0].pBt));
}

void
sqlite_obj_db_close_stop(void) {
	MON_MSG_T("DB close stop");
}

int monitor_qe_nested = 0;

void
sqlite_obj_qe_start(const char *zSql) {
	monitor_qe_nested++;
	char *query = NULL;

	/* replace in query string "new line" -> "space" */
	{
		int i;
		int len = strlen(zSql);
		query = malloc(len+1);
		for(i=0; i<=len; i++) {
			if(zSql[i]!='\n') {
				query[i] = zSql[i];
			} else {
				query[i] = ' ';
			}
		}
	}

	if(monitor_qe_nested > 1) {
		MON_MSG("QUERY start %*.s-> %s", monitor_qe_nested*2, " ", query);
	} else {
		MON_MSG_T("QUERY start %*.s-> %s", monitor_qe_nested*2, " ", query);
	}

	if(query) {
		free(query);
	}
}

void
sqlite_obj_qe_stop(void) {
	if(monitor_qe_nested > 1) {
		MON_MSG("QUERY stop  %*.s<-", monitor_qe_nested*2, " ");
	} else {
		MON_MSG_T("QUERY stop  %*.s<-", monitor_qe_nested*2, " ");
	}
	monitor_qe_nested--;
}

