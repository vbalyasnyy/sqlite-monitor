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
#include "stdlib.h"

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
file_log_t(int msg, const char* fmt, ...){
	static struct timeval prev_time;
	static char init_time = 0;
	if(!init_time) {
		gettimeofday(&prev_time, NULL);
		init_time = 1;
	}

	struct timeval now;
	gettimeofday(&now, NULL);

	if(msg) {
		va_list ap;
		va_start(ap, fmt);

		FILE *fp;
#ifdef MON_LOG_FILE_NAME
		fp = fopen(MON_LOG_FILE_NAME, "a+");
#else
		fp = fopen("/tmp/sqlilte.log", "a+");
#endif
		if (fp != NULL) {
			double diff =
				(now.tv_sec * 1000000 + now.tv_usec) -
				(prev_time.tv_sec * 1000000 + prev_time.tv_usec);
			fprintf(fp, "%5d %10.6f ", getpid(), diff/1000000);
			//fprintf(fp, "\x1b[32m%5d %10.6f\x1b[0m ", getpid(), diff/1000000);
			vfprintf(fp, fmt, ap);
			fprintf(fp, "\n");
			fclose(fp);
		}

		va_end(ap);
	}
	prev_time = now;
        return 0;
}

#define MON_MSG(fmt, arg...) file_log(fmt,##arg)
#define MON_MSG_T(msg, fmt, arg...) file_log_t(msg, fmt, ##arg)

