#include "monitor.h"

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

