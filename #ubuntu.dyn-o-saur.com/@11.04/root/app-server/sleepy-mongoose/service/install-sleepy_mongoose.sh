ln -s /root/app-server/sleepy-mongoose/service/sleepy_mongoose /etc/init.d/sleepy_mongoose
chmod +x /etc/init.d/sleepy_mongoose

useradd sleepy_mongoose

update-rc.d sleepy_mongoose defaults

/etc/init.d/sleepy_mongoose start
