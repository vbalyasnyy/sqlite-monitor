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
        fp = fopen("/tmp/sqlm.log", "a+");
#endif
        if (fp != NULL) {
                fprintf(fp, "PID=%d TIME=%f ", getpid(), ((float)clock())/CLOCKS_PER_SEC);
                vfprintf(fp, fmt, ap);
                fprintf(fp, "\n");
                fclose(fp);
        }

        va_end(ap);
        return 0;
}
#define MON_MSG(fmt, arg...) file_log(fmt,##arg)

/**********************************************/
/* LOGGER */
/**********************************************/

void sqlite_obj_db_open_start(void) {
        MON_MSG("open DB");
}

void sqlite_obj_db_open_stop(void) {
        MON_MSG("close DB");
}

