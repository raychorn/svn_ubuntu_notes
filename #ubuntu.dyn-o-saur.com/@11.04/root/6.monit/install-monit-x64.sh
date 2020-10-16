#!/bin/bash
cd /opt
if [ -d /opt/monit-5.3-linux-x64 ]; then
	echo "/opt/monit-5.3-linux-x64 already exists"
	p=$(monit -V | grep 5.3)
	if [ "$p" = "5.3" ]; then
		echo "Monit 5.3 is already installed."
	else
		p=$(which monit)
		echo "which monit $p"
		if [ -f $p ]; then
			rm -f $p
		fi
		ln -s /opt/monit-5.3-linux-x64/monit-5.3/bin/monit /usr/sbin/monit
	fi
else
	wget http://downloads.vyperlogix.com/Installer/monit/monit-5.3-linux-x64.tar.gz
	mkdir /opt/monit-5.3-linux-x64
	tar -xvzf monit-5.3-linux-x64.tar.gz -C /opt/monit-5.3-linux-x64
	if [ -d /opt/monit-5.3-linux-x64 ]; then
		rm -f /opt/monit-5.3-linux-x64.tar.gz
	fi
	if [ -f /etc/init.d/monit ]; then
		service monit stop
	fi
	p=$(which monit)
	if [ -d $p ]; then
		echo "monit is installed, check 1..."
	else
		echo "monit is not installed..."
		apt-get install monit
		if [ -f /etc/default/monit ]; then
			echo "/etc/default/monit already exists"
		else
			cd /etc/default
			wget http://downloads.vyperlogix.com/Installer/default/monit
			chmod 0644 /etc/default/monit
		fi
	fi
	p=$(which monit)
	if [ -f $p ]; then
		echo "monit is installed, check 2..."
		rm -f $p
	fi
	ln -s /opt/monit-5.3-linux-x64/monit-5.3/bin/monit /usr/sbin/monit
	if [ -f /etc/init.d/monit ]; then
		service monit start
	fi
fi
if [ -f /etc/monitrc ]; then
	echo "/etc/monitrc already exists."
else
	ln -s /etc/monit/monitrc /etc/monitrc
fi
