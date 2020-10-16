#!/bin/bash
unzip stackless-255-export.zip
cd stackless-255-export
chmod +x configure
./configure --prefix=/opt/stackless_python_2.5.5/python2.5.5
make
make test
make install
ln -s /usr/local/python2.5/bin/python /usr/bin/python2.5
ln -s /opt/python_2.5.5.7/bin/python2.5 /usr/bin/python
