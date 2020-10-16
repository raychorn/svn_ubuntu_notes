wget http://downloads.sourceforge.net/mysql-python/MySQL-python-1.2.3b1.tar.gz
tar zxvf MySQL-python-1.2.3b1.tar.gz
sudo apt-get install libmysqlclient15-dev
cd MySQL-python-1.2.3b1
~/bin/python setup.py build
~/bin/python setup.py install
