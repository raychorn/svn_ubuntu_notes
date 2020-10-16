#!/bin/bash

USER=$(whoami)

if [ ${#USER} == "0" ]; then
	echo "Nothing to do..."
else
	ps -u ${USER} -o rss,command | grep -v peruser | awk '{sum+=$1} END {print sum/1024}'
fi

