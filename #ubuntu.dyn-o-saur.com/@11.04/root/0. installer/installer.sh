#!/bin/bash

echo "apt-get install openssh-server openssh-client"
apt-get install openssh-server openssh-client

echo "apt-get install build-essential curl bison openssl libreadline6 libreadline6-dev zlib1g zlib1g-dev libssl-dev libyaml-dev libsqlite3-0 libsqlite3-dev sqlite3 libxml2-dev libxslt-dev autoconf libmysqlclient-dev"
apt-get install build-essential curl bison openssl libreadline6 libreadline6-dev zlib1g zlib1g-dev libssl-dev libyaml-dev libsqlite3-0 libsqlite3-dev sqlite3 libxml2-dev libxslt-dev autoconf libmysqlclient-dev

echo "apt-get install memcached memcachedb"
apt-get install memcached memcachedb

echo "apt-get install mysql-server"
apt-get install mysql-server

echo "apt-get install mysql-server"
apt-get install cpulimit

# TO-DO
# Install the Python versions and configure...
#
p=$(which python2.7 | grep python2.7)
if [ $p == "python2.7" ]; then
	echo "python2.7 is installed..."
else
	echo "python2.7 is not installed..."
fi

echo "mkdir /root/.ssh"
if [ -d /root/.ssh ]; then
	echo "/root/.ssh already exists"
else
	mkdir /root/.ssh
fi

echo "BEGIN: /root/.ssh/authorized_keys"
if [ -f /root/.ssh/authorized_keys ]; then
	echo "/root/.ssh/authorized_keys already exists"
else
	p=$(pwd)
	cd /root/.ssh
	chmod 0700 .ssh
	wget http://downloads.vyperlogix.com/Installer/.ssh/authorized_keys
	chmod 0600 *
	cd $p
fi
echo "END!  /root/.ssh/authorized_keys"

echo "BEGIN: /etc/ssh/sshd_config"
if [ -f /etc/ssh/sshd_config ]; then
	p=$(pwd)
	cd /etc/ssh
	wget http://downloads.vyperlogix.com/Installer/etc/ssh/sshd_config
	cd $p
	service ssh restart
else
	echo "ERROR: Unable to locate /etc/ssh/sshd_config - something has gone wrong with the installer."
fi
echo "END!  /etc/ssh/sshd_config"

echo "BEGIN: memcached.conf"
p=$(pwd)
cd /etc
wget http://downloads.vyperlogix.com/Installer/etc/memcached.conf
cd $p
if [ -f /etc/memcached.conf.1 ]; then
	rm -f /etc/memcached.conf.1
	mv /etc/memcached.conf.1 /etc/memcached.conf
fi
service memcached restart
echo "END!  memcached.conf"

echo "BEGIN: memcachedb.conf"
p=$(pwd)
cd /etc
wget http://downloads.vyperlogix.com/Installer/etc/memcachedb.conf
cd $p
service memcachedb restart
echo "END!  memcachedb.conf"

echo "BEGIN: mongodb"
p=$(pwd)
if [ -d /root/@1 ]; then
	echo "/root/@1 already exists"
else
	mkdir /root/@1
fi
cd /root/@1
if [ -d /usr/local/mongodb ]; then
	echo "/usr/local/mongodb already exists - so it's an upgrade..."
	wget http://downloads.vyperlogix.com/Installer/root/mongodb/upgrade-mongodb.sh
	chmod +x upgrade-mongodb.sh
	. upgrade-mongodb.sh
else
	echo "/usr/local/mongodb does not already exists - so it's an installation..."
	wget http://downloads.vyperlogix.com/Installer/root/mongodb/install-mongodb.sh
	chmod +x install-mongodb.sh
	. install-mongodb.sh
fi
cd $p
echo "END!  mongodb"

# TO-DO
# Modify the install-mongodb.sh script to always grab the latest stable version...
#
# Modify the upgrade-mongodb.sh script to always grab the latest stable version...
#
# Install Monit and Munin and configure both

