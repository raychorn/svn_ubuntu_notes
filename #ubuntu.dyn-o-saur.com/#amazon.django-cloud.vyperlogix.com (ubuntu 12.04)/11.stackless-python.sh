#!/bin/bash

if [ -d "/opt" ]; then 
	echo
else
	mkdir /opt
fi

if [ -d "/opt/stackless" ]; then 
	echo
else
	mkdir /opt/stackless
fi

cd /opt/stackless
wget http://pts-mini-gpl.googlecode.com/svn/trunk/staticpython/release/stacklessco2.7-static

ln -s /opt/stackless/stacklessco2.7-static /usr/bin/stackless

