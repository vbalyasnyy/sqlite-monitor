#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <time.h>

#define SQLM_FILE_LEN 30

int
tmp_log(const char* fmt, ...){

        va_list ap;
        va_start(ap, fmt);

        FILE *fp;
        //fp = fopen("/tmp/sqlite_log.log", "a+");
	char fname[SQLM_FILE_LEN];
//	sprintf(fname, "/tmp/.%d.sqlm", getpid());
	sprintf(fname, "/tmp/sqlm.log");
	fp = fopen(fname, "a+");
        if (fp != NULL) {
		fprintf(fp, "PID=%d TIME=%f ", getpid(), ((float)clock())/CLOCKS_PER_SEC);
                vfprintf(fp, fmt, ap);
                fprintf(fp, "\n");
                fclose(fp);
        }

        va_end(ap);
        return 0;
}
#define DBG(fmt, arg...) tmp_log(fmt,##arg)

