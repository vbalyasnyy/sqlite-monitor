SQLITE_VERSION=3080500

echo ">>> GET SQLITE SOURCES ..."
ls ./sqlite-autoconf-${SQLITE_VERSION}.tar.gz
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
cp ./empty.sqlite ../
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
./sqlite3 ./empty.sqlite "SELECT * from test;" &
./sqlite3 ./empty.sqlite "SELECT * from test;" &
./sqlite3 ./empty.sqlite "SELECT * from test;" &

sleep 3
cat /tmp/sqlite.log
