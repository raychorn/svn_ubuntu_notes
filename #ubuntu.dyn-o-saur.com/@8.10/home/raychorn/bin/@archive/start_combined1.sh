export PYTHONPATH=$HOME:$HOME/combined:$HOME/lib/python2.5/site-packages:$HOME/python25/lib/VyperLogixLib-1.0-py2.5.egg:$HOME/python25/lib/VyperLogixPyaxLib-1.0-py2.5.egg:$HOME/python25/lib/SQLAlchemy-0.5.3-py2.5.egg:/usr/lib/python2.5/site-packages:/var/lib/python-support/python2.5:$PYTHONPATH

python $HOME/combined/manage.pyc runfcgi method=threaded host=127.0.0.1 port=9000 protocol=scgi
