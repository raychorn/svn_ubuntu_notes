/root/bin/start_cherokee_admin.sh

#/home/raychorn/bin/start_vyperlogix.sh
#/home/raychorn/bin/start_pypi_vyperlogix.sh
#/home/raychorn/bin/start_pyeggs.sh
#/home/raychorn/bin/start_todo.sh

/root/zoneclient/run.sh

if [ -f /etc/monit/memcachedb.monitrc@ ]; then
	service memcachedb stop
fi


