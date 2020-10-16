#!/bin/bash

#http://www.debuntu.org/2006/05/20/54-how-to-subversion-svn-with-apache2-and-dav

apt-get install apache2 apache2.2-common apache2-mpm-prefork apache2-utils libexpat1 ssl-cert -V
apt-get install libapache2-mod-proxy-html libxml2-dev -V

apt-get install subversion libapache2-svn -V

apt-get install cpulimit -V

# Do the config for Apache2 and Subversion

#/etc/init.d/apache2 restart
