#!/bin/bash

DATE=$(/root/bin/date.sh)

pid=$(ps aux | grep cpulimit | grep -v grep | awk '{print $2}' | tail -n 1)
#echo "pid=${pid}"
#echo "#pid=${#pid}"
target=$(ps -ef | grep ${pid} | grep " -p " | grep -v grep | awk '{print $10}' | tail -n 1)
if [ ${#target} == "0" ]; then
	echo "${DATE} Nothing to do."
else
	#echo "target=${target}"
	#echo "#target=${#target}"
	tgt=$(ps -ef | grep ${target} | grep -v grep | awk '{print $10}' | tail -n 1)
	p=$(ps aux | grep ${target} | grep -v grep | awk '{print $2}' | tail -n 1)
	#echo "tgt=${tgt}"
	#echo "#tgt=${#tgt}"
	#echo "p=${p}"
	#echo "#p=${#p}"
	if [ ${tgt} == ${target} ]; then
		echo "The target pid is the same as the expected pid on the cpulimits therefore the process being watched by the cpulimits is not running."
		if [ ${#p} == "0" ]; then
			echo "${DATE} Nothing to do."
		else
			echo "${DATE} kill -9 ${p}"
		fi
	else
		echo "${DATE} Nothing to do because the target pid is not the same as the expected pid on the cpulimits therefore the process being watched by the cpulimits is running."
	fi
fi
