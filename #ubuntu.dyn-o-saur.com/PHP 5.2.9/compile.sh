tar -xjf php-5.2.9.tar.bz2

cd php-5.2.9

apt-get install php5-dev libmysqlclient15-dev bzip2 libcurl3 curl libpng12-dev libfreetype6-dev libmcrypt4 libmcrypt-dev libmhash2 libmhash-dev libxml2-dev libxslt1-dev apache2-prefork-dev libjpeg62-dev freetype2 libxft libxft-dev libcurl4-gnutls-dev

./configure --with-apxs2=/usr/bin/apxs2 --with-config-file-path=/etc/php5/apache2/ --with-mysql --enable-inline-optimization --disable-debug --enable-bcmath --enable-calendar --enable-ctype --enable-dbase --enable-discard-path --enable-exif --enable-force-cgi-redirect --enable-ftp --enable-gd-native-ttf --with-ttf --enable-shmop --enable-sigchild --enable-sysvsem --enable-sysvshm --enable-wddx --with-zlib=yes --with-openssl --with-xsl  --with-gd --with-gettext --with-mcrypt --with-mhash --enable-sockets --enable-mbstring=all --enable-mbregex --enable-zend-multibyte --enable-exif --enable-soap --enable-pcntl --with-mysqli --with-mime-magic --with-iconv --with-pdo-mysql --with-freetype-dir=/usr/include/freetype2/freetype

make

make install
