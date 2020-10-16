#!/bin/bash

svn update --username molten_deploy --password peekab00 $HOME/vyperlogix_site

chmod 0744 $HOME/vyperlogix_site/djangocerise_1_2/*.sh 
chmod 0744 $HOME/vyperlogix_site/manage.pyc 



