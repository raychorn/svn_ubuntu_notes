#!/bin/bash
wget http://peak.telecommunity.com/dist/virtual-python.py
python2.7 virtual-python.py
mkdir ~/local
ln -s ~/lib ~/local/lib
ln -s ~/include ~/local/include
wget http://peak.telecommunity.com/dist/ez_setup.py
~/bin/python ez_setup.py
~/bin/easy_install pip