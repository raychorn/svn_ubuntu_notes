#!/bin/bash

cd /var/local/svn
service apache2 stop
if [ -d /var/local/svn/@repo1 ]; then
	rm -R -f /var/local/svn/@repo1
fi
if [ -d /var/local/svn/repo1 ]; then
	mv /var/local/svn/repo1 /var/local/svn/@repo1
fi
7z x "/root/@svn/#svn.7z"
mv "/var/local/svn/#svn" /var/local/svn/repos
chown www-data:www-data -R -f /var/local/svn/repo1
ln -s /var/local/svn/repos/repo1 /var/local/svn/repo1
service apache2 start
