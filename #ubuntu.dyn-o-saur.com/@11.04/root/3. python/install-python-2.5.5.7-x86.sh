#!/bin/bash
wget http://downloads.activestate.com/ActivePython/releases/2.5.5.7/ActivePython-2.5.5.7-linux-x86.tar.gz
tar -xvzf ActivePython-2.5.5.7-linux-x86.tar.gz
cd ActivePython-2.5.5.7-linux-x86
./install.sh -I /opt/python_2.5.5.7 -v
ln -s /opt/python_2.5.5.7/bin/python2.5 /usr/bin/python2.5
