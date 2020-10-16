echo "BEGIN: stop-cherokee-admin"

if [ -f /var/run/cherokee-admin.pid ]; then
	x=$(cat /var/run/cherokee-admin.pid)
	kill -9 $x
	rm -f /var/run/cherokee-admin.pid
	rm -f /root/cherokee-admin/cherokee-admin.txt
	p=$(ps -ef | grep /cherokee/admin)
	if [ ${#p} == "0" ]; then
		echo "Nothing to do."
	else
		kill -9 $p
	fi

else
	echo "Nothing to do !!!"
fi

echo "END!  stop-cherokee-admin"


