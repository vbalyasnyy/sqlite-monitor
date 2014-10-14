MONITOR_VER=3008005

cd ..
## CHECK SQLITE VERSION
SQLITE_VER=`grep -r "#define SQLITE_VERSION_NUMBER" ./ | head -1 | awk '{print $3}'`
if [ "x$SQLITE_VER" != "x$MONITOR_VER" ];
then
	echo "Error: version of sqlite3-$SQLITE_VER, version of monitor-$MONITOR_VER"
	exit
fi

## COPY SOURCE OF MONITOR
cp ./monitor/monitor.h ./
cp ./monitor/sqlite_monitor.h ./

## PATCHED SQLITE SOURCES
patch ./sqlite3.c < ./monitor/monitor.patch

## ECHO FINISH MEASSAGE
echo "Sqlite Monitor successefully installed, try to build sqlite."
echo "If you have a problem, contact me on https://github.com/vbalyasnyy/sqlite-monitor."
