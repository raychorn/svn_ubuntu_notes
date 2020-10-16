x=$(which htpasswd)
if [ -f $x ]; then
	echo "apache2-utils is already installed !!!"
else
	apt-get install apache2-utils
fi

echo "admin"
htpasswd -c /var/cache/munin.htpasswd admin

echo "rhorn"
htpasswd -c /var/cache/munin.htpasswd rhorn

echo "raychorn"
htpasswd -c /var/cache/munin.htpasswd raychorn

