#!/bin/bash

svn update --username molten_deploy --password peekab00 $HOME/pypi_info

chmod 0744 $HOME/pypi_info/djangocerise/*.sh 
chmod 0744 $HOME/pypi_info/manage.pyc 
chmod 0744 $HOME/pypi_info/feeds/*.sh 



