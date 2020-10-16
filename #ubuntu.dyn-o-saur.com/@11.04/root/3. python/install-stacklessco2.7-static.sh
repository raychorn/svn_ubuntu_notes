#!/bin/bash
mkdir /opt/stackless
cd /opt/stackless
curl http://pts-mini-gpl.googlecode.com/svn/trunk/staticpython/d.sh | bash /dev/stdin stacklessco2.7-static
ln -s /opt/stackless/stacklessco2.7-static /usr/bin/stacklessco2.7-static
