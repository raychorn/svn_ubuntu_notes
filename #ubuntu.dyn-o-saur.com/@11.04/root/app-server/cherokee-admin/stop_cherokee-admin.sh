echo "BEGIN: stop-cherokee-admin"

if [ -f /var/run/cherokee-admin.pid ]; then
	x=$(cat /var/run/cherokee-admin.pid)
	kill -9 $x
	rm -f /var/run/cherokee-admin.pid
	rm -f /root/app-server/cherokee-admin/cherokee-admin.txt
else
	echo "Nothing to do !!!"
fi

echo "END!  stop-cherokee-admin"


