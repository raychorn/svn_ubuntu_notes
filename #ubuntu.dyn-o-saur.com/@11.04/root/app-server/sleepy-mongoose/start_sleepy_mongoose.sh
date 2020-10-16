. /root/bin/PYTHONPATH

echo "BEGIN: start-sleepy_mongoose"

if [ -f /var/run/sleepy_mongoose.pid ]; then
	echo "Nothing to do !!!"
else
	echo Starting sleepy_mongoose port=27080 using -m 127.0.0.1:27017
	python2.5 /root/app-server/sleepy-mongoose/sleepy_mongoose.py -m 127.0.0.1:27017 -x &
	ps aux | grep sleepy_mongoose | grep -v grep | awk '{print $2}' | tail -n 1 > /var/run/sleepy_mongoose.pid
fi

echo "END! start-sleepy_mongoose"


