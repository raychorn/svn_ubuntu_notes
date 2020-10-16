deb http://repos.zend.com/deb/ce ce non-free

wget http://repos.zend.com/deb/zend.key -O- |apt-key add -

apt-get update 

aptitude install zend-ce

