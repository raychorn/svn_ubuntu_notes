export PYTHONPATH=${PYTHONPATH}:~/python/libs/2.5/vyperlogix.zip:/usr/share/pyshared

x=$(ps -ef | grep cherokee-admin -c)
echo $x
if [ ${x} -le 1 ]; then
	echo Starting cherokee-admin --port=9090
	rm -f app-server/cherokee-admin.txt
	cherokee-admin -b0.0.0.0 -p9090 >>app-server/cherokee-admin.txt &
fi

python2.5 /root/python/bin/pid.pyc --pname=cherokee-admin -verbose
