if [ -f /etc/init.d/app-server ]; then
	echo "Nothing to do !!!"
else
	ln -s /root/app-server/app-server /etc/init.d/app-server
	update-rc.d app-server defaults
fi

