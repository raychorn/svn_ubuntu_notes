export PYTHONPATH=$HOME:$HOME/combined:$HOME/python25/lib/VyperLogixLib-1.0-py2.5.egg:$HOME/python25/lib/VyperLogixPyaxLib-1.0-py2.5.egg:$HOME/python25/lib/SQLAlchemy-0.5.3-py2.5.egg:$PYTHONPATH

echo $PYTHONPATH

cd $HOME/combined/djangocerise_1_2/

python stop.pyc --kill=127.0.0.1:9000-9002
