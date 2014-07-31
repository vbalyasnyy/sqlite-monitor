SQLITE_VERSION=3080500

wget www.sqlite.org/2014/sqlite-autoconf-${SQLITE_VERSION}.tar.gz
tar xzf sqlite-autoconf-${SQLITE_VERSION}.tar.gz
cd sqlite-autoconf-${SQLITE_VERSION}/

cp -r ../sqlite-monitor/monitor/ ./
cd monitor/
./install.sh

cd ..
./configure
make

if [ "x$?" != "x0" ];
then
        echo "Error: some problem with build sqlite ${SQLITE_VERSION}."
        exit
fi

#rm -rf ./sqlite-autoconf-${SQLITE_VERSION}*
