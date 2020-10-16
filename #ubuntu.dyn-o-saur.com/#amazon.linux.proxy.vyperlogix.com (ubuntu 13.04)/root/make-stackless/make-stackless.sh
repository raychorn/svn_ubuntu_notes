apt-get install libreadline-dev
apt-get build-dep python2.7

cd stackless    
./configure --prefix=/opt/stackless --enable-unicode=ucs4
make
    
make install

sudo ln -s /opt/stackless/bin/python2.5 /usr/bin/stackless
