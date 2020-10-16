# ~/.profile: executed by Bourne-compatible login shells.

if [ "$BASH" ]; then
  if [ -f ~/.bashrc ]; then
    . ~/.bashrc
  fi
fi

if [ -d "/root/bin" ] ; then
    PATH="/root/bin:/root/local/bin:/usr/bin:$PATH"
fi

mesg n

if [ -d /root/lib/python2.5 ]; then
	if [ -d /root/lib/python2.5/site-packages ]; then
		export PYTHONPATH=/root/lib/python2.5/site-packages
	fi
	if [ -f /root/python/2.5/vyperlogix_2_5_5.zip ]; then
		export PYTHONPATH=$PYTHONPATH:/root/python/2.5/vyperlogix_2_5_5.zip
	fi
fi

if [ -d /root/lib/python2.7 ]; then
	if [ -d /root/lib/python2.7/site-packages ]; then
		export PYTHONPATH=/root/lib/python2.7/site-packages
	fi
	if [ -d /usr/local/lib/python2.7/dist-packages ]; then
		export PYTHONPATH=$PYTHONPATH:/usr/local/lib/python2.7/dist-packages
	fi
	if [ -d /usr/local/lib/python2.7/site-packages ]; then
		export PYTHONPATH=$PYTHONPATH:/usr/local/lib/python2.7/site-packages
	fi
	if [ -d /usr/lib/python2.7 ]; then
		export PYTHONPATH=$PYTHONPATH:/usr/lib/python2.7
	fi
	if [ -d /usr/lib/python2.7/dist-packages ]; then
		export PYTHONPATH=$PYTHONPATH:/usr/lib/python2.7/dist-packages
	fi
	if [ -d /usr/lib/python2.7/site-packages ]; then
		export PYTHONPATH=$PYTHONPATH:/usr/lib/python2.7/site-packages
	fi
	if [ -d /usr/lib/pymodules/python2.7 ]; then
		export PYTHONPATH=$PYTHONPATH:/usr/lib/pymodules/python2.7
	fi
	if [ -d /opt/python_2.7.2.5/lib/python2.7 ]; then
		export PYTHONPATH=$PYTHONPATH:/opt/python_2.7.2.5/lib/python2.7
	fi
	if [ -d /opt/python_2.7.2.5/lib/python2.7 ]; then
		export PYTHONPATH=$PYTHONPATH:/opt/python_2.7.2.5/lib/python2.7
	fi
	if [ -f /root/python/2.7/vyperlogix_2_7_0.zip ]; then
		export PYTHONPATH=$PYTHONPATH:/root/python/2.7/vyperlogix_2_7_0.zip
	fi
fi

export PYTHONPATH=$PYTHONPATH:/usr/share/pyshared

#export PYTHONPATH=/usr/lib/python2.7/dist-packages/apt
#:/usr/share/python-support/python-webpy:/usr/share/python-support/python-openssl:/usr/share/python-support/python-mysqldb:/usr/share/python-support/python-gobject:/usr/share/python-support/python-gnupginterface:/usr/share/python-support/python-flup:/usr/share/python-support/python-dbus:/usr/share/python-support/python-cheetah

