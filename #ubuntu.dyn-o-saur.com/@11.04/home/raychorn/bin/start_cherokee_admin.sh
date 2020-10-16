x=$(ps -ef | grep cherokee-admin -c)
echo $x
if [ ${x} -le 1 ]; then
	echo Starting cherokee-admin --port=9999
	rm -f /home/raychorn/bin/cherokee-admin.txt
	#cherokee-admin CHEROKEE_TRACE=all cherokee-admin -x -b --port=9999 >>/home/raychorn/bin/cherokee-admin.txt &
	nohup cherokee-admin -b192.168.1.105 -p9999 -T32 >>/home/raychorn/bin/cherokee-admin.txt &
	#python /usr/share/cherokee/admin/server.py /tmp/cherokee-admin-scgi.socket /etc/cherokee/cherokee.conf &
	#cherokee-admin CHEROKEE_TRACE=all cherokee-admin -x -b --port=9090
fi

#export PYTHONPATH=/home/admin/lib/python2.5/site-packages:/home/admin/lib/python2.5/site-packages/VyperLogixLib-1.0-py2.5.egg:/usr/lib/python2.5/site-packages/tornado:/usr/share/python-support/python-openssl:/usr/share/python-support/python-mysqldb:/usr/share/python-support/python-gobject:/usr/share/python-support/python-gnupginterface:/usr/share/python-support/python-flup:/usr/share/python-support/python-dbus:/usr/share/python-support/python-cheetah
#python2.5 /home/admin/bin/pid/pid.pyc --pname=cherokee-admin -verbose

#/etc/init.d/monit restart
