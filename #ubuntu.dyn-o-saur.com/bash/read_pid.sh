function isRunning {
	PID=$(cat $1)
	x=$(ps -ef | grep ${PID} -c)
	if [ ${x} > 0 ]; then
		echo "Still running :: ${PID} :: $1"
	fi
}  

files=$(ls /var/run/django/*.pid)
for i in ${files}; do
	isRunning $i
done
