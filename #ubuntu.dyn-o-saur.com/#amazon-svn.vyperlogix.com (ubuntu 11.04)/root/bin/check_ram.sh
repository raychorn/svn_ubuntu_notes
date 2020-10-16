#!/bin/bash

USER=$(whoami)

if [ ${#USER} == "0" ]; then
	echo "Nothing to do..."
else
	ps -u ${USER} -o pid,rss,command
fi
