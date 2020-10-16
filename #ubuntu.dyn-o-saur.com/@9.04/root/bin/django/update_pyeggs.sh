#!/bin/bash

svn update --username molten_deploy --password peekab00 /var/django/pyeggs

chmod 0744 /var/django/pyeggs/djangocerise_1_2/*.sh 
chmod 0744 /var/django/pyeggs/manage.pyc 



