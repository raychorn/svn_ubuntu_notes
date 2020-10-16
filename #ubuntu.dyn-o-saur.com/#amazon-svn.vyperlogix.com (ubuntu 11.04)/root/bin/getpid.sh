#!/bin/bash

pid=$(ps aux | grep svnHotBackups.py | grep -v grep | awk '{print $2}' | tail -n 1)

echo ${#pid}

if [ ${#pid} == "0" ]; then
	echo "Not yet running..."
else
	echo "Running now !"
fi

