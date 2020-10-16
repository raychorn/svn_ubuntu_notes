function booster {
	PID=$(cat $1)
	x=$(ps -ef | grep ${PID} -c)
	if [ ${x} ]; then
		renice -5 -p ${PID}
	fi
}  

files=$(ls /var/run/django/*.pid)
for i in ${files}; do
	booster $i
done
