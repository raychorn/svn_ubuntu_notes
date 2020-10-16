export PYTHONPATH=/home/admin/lib/python2.5/site-packages:/home/admin/lib/python2.5/site-packages/VyperLogixLib-1.0-py2.5.egg:/usr/lib/python2.5/site-packages/tornado:/usr/share/python-support/python-openssl:/usr/share/python-support/python-mysqldb:/usr/share/python-support/python-gobject:/usr/share/python-support/python-gnupginterface:/usr/share/python-support/python-flup:/usr/share/python-support/python-dbus:/usr/share/python-support/python-cheetah

x=$(ps -ef | grep cherokee-admin -c)
echo $x
if [ ${x} -le 1 ]; then
	echo Starting cherokee-admin --port=9090
	rm -f /home/admin/bin/cherokee-admin.txt
	cherokee-admin -b --port=9090 >>/home/admin/bin/cherokee-admin.txt &
fi

python /home/admin/bin/pid/pid.pyc --pname=cherokee-admin -verbose

#/etc/init.d/monit restart
