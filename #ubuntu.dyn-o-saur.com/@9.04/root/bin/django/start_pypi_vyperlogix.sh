export PYTHONPATH=/var/django/pypi_info:$HOME/lib/python2.5/site-packages:$HOME/python25/lib/VyperLogixLib-1.0-py2.5.egg:$HOME/python25/lib/VyperLogixPyaxLib-1.0-py2.5.egg:$HOME/python25/lib/SQLAlchemy-0.5.3-py2.5.egg:$HOME/python25/lib/Django-0.96.3-py2.5.egg:$PYTHONPATH

python /var/django/pypi_info/manage.pyc runfcgi method=threaded host=127.0.0.1 port=9300 protocol=scgi pidfile=/var/run/pypi_info.pid
