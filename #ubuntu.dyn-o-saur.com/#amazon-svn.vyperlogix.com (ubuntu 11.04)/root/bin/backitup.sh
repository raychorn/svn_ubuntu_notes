#!/bin/bash

if [ -d /root/bin/logs ]; then
	echo "/root/bin/logs already exists"
else
	mkdir /root/bin/logs
fi

/root/bin/backup.sh >./logs/backup.log 2>&1 &
