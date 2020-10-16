#!/bin/bash

svn update --username molten_deploy --password peekab00 $HOME/combined

chmod 0744 $HOME/combined/djangocerise_1_2/*.sh 
chmod 0744 $HOME/combined/manage.pyc 



