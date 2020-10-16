#!/bin/bash

fname=repo1-13966
tar xvfz $fname.tar.gz
service apache2 stop
unlink /var/local/svn/repo1
chown www-data:www-data -R -f /root/@1/$fname
mv /root/@1/$fname /var/local/svn/repo1
service apache2 start
