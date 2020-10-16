#!/bin/bash
wget http://downloads.activestate.com/ActivePython/releases/2.7.2.5/ActivePython-2.7.2.5-linux-x86_64.tar.gz
tar -xvzf ActivePython-2.7.2.5-linux-x86.tar.gz
cd ActivePython-2.7.2.5-linux-x86
./install.sh -I /opt/python_2.7.2.5 -v
if [ -f /usr/bin/python2.7 ]
then
	mv /usr/bin/python2.7 /usr/bin/python2.7.1
	unlink /usr/bin/python
	ln -s /usr/bin/python2.7.1 /usr/bin/python
fi
ln -s /opt/python_2.7.2.5/bin/python2.7 /usr/bin/python2.7