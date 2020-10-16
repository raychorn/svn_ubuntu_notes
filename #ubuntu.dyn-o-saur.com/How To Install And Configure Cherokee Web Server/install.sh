apt-get install build-essential

apt-get install php5-cgi

apt-get install php5-mysql

cd ~

mkdir @1

cd @1

wget http://www.cherokee-project.com/download/trunk/cherokee-latest-svn.tar.gz

tar -zxvf cherokee-latest-svn.tar.gz

cd cherokee-0.99.8b3032

./configure --localstatedir=/var --prefix=/usr --sysconfdir=/etc --with-wwwroot=/var/www

make

make install