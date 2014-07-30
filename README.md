sqlite-monitor
==============

Tool to monitor sqlite objects: connections, tables, queries and etc.

Install
------------------------------------------
Need to build sqlite with implemented patch:

1) Add monitor messages
-----------------------
slite3.c:
``` ...........
 #include <sqlite_monitor.h>   <+++
 ...........                   
 DBG(...                       <+++
 ...........                   
 DBG(...                       <+++
 ..........                    
 ```

2) Add directory with monitor files and config file:
./monitor/*
./monitor/sqlite_monitor.conf:
```
LOG_TYPE = FILE|DAEMON
OBJECT_LIST = CONNECTION|DBASE|TABLE|QUEARIE|
VERBOSE_LEVEL = EVENT|TIME
```

System components:
---------------------
./monitor/sqlite_monitor.h:
```
// LOG TYPE
#define MON_LOG_TYPE FILE //DAEMON
#define MON_LOG_FILE_NAME "/tmp/sqlite.log" #by default

// SQLITE_OBJECT
#define MON_OBJ_CON ON // DB connections
#define MON_OBJ_QUE ON // SQL queries

// VERBOSE LEVEL
#define MON_VERBOSE_LEVEL // 1. Just event message.
                          // 2. Time of instruction
```
./monitor/sqlite_monitor_logger.c:
```
tmp_write(...){...};
send_daemon(...){...};

#ifdef MON_LOG_TYPE
 #define MON_MSG(...) tmp_write(...)
#else
 #define MON_MSG(...) send_daemon(...)
#endif
```

./monitor/sqlite_monitor_obj_conn.c: //same sources for every monitoring objects
```
print_connect(...){MON_MSG("DB name %s", ...DBname); ...};
start_connect(...){...};
stop_connect(...){...};
```
./monitor/sqlite_monitor_implement.sh:
```
patch < -p1 ./header_include.patch
patch < -p1 ./connection_include.patch
patch < -p1 ./queries_include.patch
```

