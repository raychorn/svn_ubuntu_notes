echo "BEGIN: stop-sleepy_mongoose"

if [ -f /var/run/sleepy_mongoose.pid ]; then
	x=$(cat /var/run/sleepy_mongoose.pid)
	kill -9 $x
	rm -f /var/run/sleepy_mongoose.pid
else
	echo "Nothing to do !!!"
fi

echo "END!  stop-sleepy_mongoose"


