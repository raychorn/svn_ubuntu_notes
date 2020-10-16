#!/bin/sh 
### Set PATH ### 
PHP_CGI=/usr/bin/php-cgi  
PHP_FCGI_CHILDREN=5 
PHP_FCGI_MAX_REQUESTS=100000 
### no editing below ### 
export PHP_FCGI_CHILDREN 
export PHP_FCGI_MAX_REQUESTS 
exec $PHP_CGI -b 127.0.0.1:5000
