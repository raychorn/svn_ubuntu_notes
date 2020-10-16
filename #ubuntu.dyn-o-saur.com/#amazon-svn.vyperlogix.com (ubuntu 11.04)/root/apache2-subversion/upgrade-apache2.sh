#!/bin/bash

wget http://apache.mirrors.tds.net//httpd/httpd-2.2.21.tar.gz
tar xvfz httpd-2.2.21.tar.gz
cd httpd-2.2.21
./configure
make
service apache2 stop
make install
service apache2 start
