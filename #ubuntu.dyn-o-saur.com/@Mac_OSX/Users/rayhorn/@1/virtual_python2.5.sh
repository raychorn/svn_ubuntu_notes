#!/bin/bash
curl http://peak.telecommunity.com/dist/virtual-python.py > virtual-python.py
/Users/rayhorn/bin/python virtual-python.py
curl http://peak.telecommunity.com/dist/ez_setup.py > ez_setup.py
/Users/rayhorn/bin/python ez_setup.py --install-dir=/Users/rayhorn/lib/python2.5/site-packages
easy_install --install-dir=/Users/rayhorn/lib/python2.5/site-packages pip
