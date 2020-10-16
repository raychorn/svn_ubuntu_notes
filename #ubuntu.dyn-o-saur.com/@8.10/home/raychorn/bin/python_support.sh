export PYTHONPATH=

files=$(ls /usr/share/python-support/ | grep python-)
for i in ${files}; do
	echo /usr/share/python-support/$i
	export PYTHONPATH=/usr/share/python-support/$i:$PYTHONPATH
done

echo $PYTHONPATH