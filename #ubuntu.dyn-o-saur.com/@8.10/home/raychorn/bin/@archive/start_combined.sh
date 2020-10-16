export PYTHONPATH=$HOME:$HOME/combined:$HOME/python25/lib/VyperLogixLib-1.0-py2.5.egg:$HOME/python25/lib/VyperLogixPyaxLib-1.0-py2.5.egg:$HOME/python25/lib/SQLAlchemy-0.5.3-py2.5.egg:$HOME/python25/lib/Django-0.96.3-py2.5.egg:$PYTHONPATH

echo $PYTHONPATH

if [ -d "/var/run/django" ] ; then {
    echo
} else {
    echo "sudo some stuff."
    sudo mkdir /var/run/django
    sudo chmod 0777 /var/run/django
}
fi

cd $HOME/combined/djangocerise_1_2/

python webserver.pyc --conf myprojectconf --host 127.0.0.1:9000 --daemon=1
python webserver.pyc --conf myprojectconf --host 127.0.0.1:9001 --daemon=1
python webserver.pyc --conf myprojectconf --host 127.0.0.1:9002 --daemon=1

