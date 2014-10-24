WORKDIR=$PWD
echo "# >Build sqlite application example..."

echo "# >Generate fwrap.c..."
cd ./monitor/
rm fwrap.c
./wrapper_gen.sh API.txt run_time

echo "# >Generate shared lib..."
rm libfwrap.so 
gcc -fPIC -rdynamic -c fwrap.c
gcc -shared -o libfwrap.so fwrap.o -lc -ldl

echo "# >Build test application..."
cd ./db_examples/
gcc test.c -o appl -I${WORKDIR}/sqlite-autoconf-3080500/ -L${WORKDIR}/sqlite-autoconf-3080500/.libs/ -lsqlite3
#./appl
echo "# >Launch test application with wrapper..."
rm /tmp/sqlite.log
LD_LIBRARY_PATH=. LD_PRELOAD=../libfwrap.so ./appl "test1.sqlite"&
LD_LIBRARY_PATH=. LD_PRELOAD=../libfwrap.so ./appl "test2.sqlite"&
LD_LIBRARY_PATH=. LD_PRELOAD=../libfwrap.so ./appl "test3.sqlite"&
if [ "x$?" == "x0" ]; then
	echo "# >Look's GOOD..."
else
	echo "# >FAIL..."
	exit 1
fi
sleep 2
cat /tmp/sqlite.log | less -r
