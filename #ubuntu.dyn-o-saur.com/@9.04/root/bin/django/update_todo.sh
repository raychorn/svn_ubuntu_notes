#!/bin/bash

svn update --username molten_deploy --password peekab00 /var/django/todo

chmod 0744 /var/django/todo/djangocerise_1_2/*.sh 
chmod 0744 /var/django/todo/manage.pyc 



