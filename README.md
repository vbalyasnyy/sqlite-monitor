sqlite-monitor
==============

Tool to monitor sqlite objects: connections, tables, queries and etc.

Install
------------------------------------------
Need to build sqlite with implemented patch:

1) Add monitor messages
-----------------------
slite3.c
________________________________|
| ...........                   |
| #include <sqlite_monitor.h>   |<+++
| ...........                   |
| DBG(...                       |<+++
| ...........                   |
| DBG(...                       |<+++
| ..........                    |
________________________________|

2) Add directory with monitor files and config file:
./sqlite_monitor.h
./monitor/*

sqlite_monitor.conf:
LOG_TYPE = FILE|DAEMON
OBJECT_LIST = CONNECTION|DBASE|TABLE|QUEARIE|
VERBOSE_LEVEL = EVENT|TIME

