#!/bin/bash

svn update --username molten_deploy --password peekab00 /var/django/pypi_info

chmod 0744 /var/django/pypi_info/djangocerise/*.sh 
chmod 0744 /var/django/pypi_info/manage.pyc 
chmod 0744 /var/django/pypi_info/feeds/*.sh 



