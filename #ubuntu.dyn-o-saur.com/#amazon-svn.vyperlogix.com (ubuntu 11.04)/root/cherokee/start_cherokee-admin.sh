if [ -f /root/bin/PYTHONPATH ]; then
	. /root/bin/PYTHONPATH
fi

echo "BEGIN: start-cherokee_admin"

if [ -f /var/run/cherokee-admin.pid ]; then
	echo "Nothing to do !!!"
else
	echo Starting cherokee-admin --port=9090
	rm -f /root/cherokee/cherokee-admin.txt
	cherokee-admin --bind=0.0.0.0 --port=9090 >>/root/cherokee/cherokee-admin.txt &
	ps aux | grep cherokee-admin | grep -v grep | awk '{print $2}' | tail -n 1 > /var/run/cherokee-admin.pid
fi

echo "END! start-cherokee_admin"


