export PYTHONPATH=/root/python25/lib/VyperLogixLib-1.0-py2.5.egg:/root/python25/lib/VyperLogixPyaxLib-1.0-py2.5.egg:$PYTHONPATH

x=$(ps -ef | grep cherokee-admin -c)
echo $x
if [ ${x} -le 1 ]; then
	echo Starting cherokee-admin --port=9090
	rm -f /root/bin/cherokee-admin.txt
	cherokee-admin -b --port=9090 >>/root/bin/cherokee-admin.txt &
fi

python2.5.4 /root/bin/pid/pid.pyc --pname=cherokee-admin -verbose

/etc/init.d/monit restart
