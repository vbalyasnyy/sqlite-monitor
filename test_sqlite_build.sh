SQLITE_VERSION=3080500

echo ">>> GET SQLITE SOURCES ..."
ls ./sqlite-autoconf-${SQLITE_VERSION}.tar.gz > /dev/null 2>&1
if [ "x$?" != "x0" ];
then
	wget www.sqlite.org/2014/sqlite-autoconf-${SQLITE_VERSION}.tar.gz
fi
rm -r ./sqlite-autoconf-${SQLITE_VERSION}/
tar xzf sqlite-autoconf-${SQLITE_VERSION}.tar.gz

echo ">>> SETUP MONITOR..."
cd sqlite-autoconf-${SQLITE_VERSION}/
cp -r ../monitor/ ./
cd ./monitor/
cp ./db_examples/test*.sqlite ../
./install.sh

echo ">>> TRY TO BUILD SQLITE WITH MONITOR..."
cd ..
./configure
make

if [ "x$?" != "x0" ];
then
        echo "Error: some problem with build sqlite ${SQLITE_VERSION}."
        exit
fi

echo ">>> TEST MONITOR..."
rm /tmp/sqlite.log
cp -r ../monitor/ ./
./sqlite3 ./test1.sqlite "SELECT * from test;" &
./sqlite3 ./test2.sqlite "SELECT * from test;" &
./sqlite3 ./test3.sqlite "SELECT * from test;" &
#3
./sqlite3 ./test1.sqlite "SELECT * from test;" &
./sqlite3 ./test2.sqlite "SELECT * from test;" &
./sqlite3 ./test3.sqlite "SELECT * from test;" &
#6
./sqlite3 ./test1.sqlite "SELECT * from test;" &
./sqlite3 ./test2.sqlite "SELECT * from test;" &
./sqlite3 ./test3.sqlite "SELECT * from test;" &
#9
./sqlite3 ./test1.sqlite "SELECT * from test;" &
./sqlite3 ./test2.sqlite "SELECT * from test;" &
./sqlite3 ./test3.sqlite "SELECT * from test;" &
#12

sleep 3
cat /tmp/sqlite.log
