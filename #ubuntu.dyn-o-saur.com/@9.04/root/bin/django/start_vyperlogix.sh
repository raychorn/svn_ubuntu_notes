export PYTHONPATH=/var/django/vyperlogix_site:$HOME/lib/python2.5/site-packages:$HOME/python25/lib/VyperLogixLib-1.0-py2.5.egg:$HOME/python25/lib/VyperLogixPyaxLib-1.0-py2.5.egg:$HOME/python25/lib/SQLAlchemy-0.5.3-py2.5.egg:$HOME/python25/lib/Django-0.96.3-py2.5.egg:$PYTHONPATH

export DJANGO_SETTINGS_MODULE=settings

python /var/django/vyperlogix_site/manage.pyc runfcgi method=threaded host=127.0.0.1 port=9100 protocol=scgi pidfile=/var/run/vyperlogix_site.pid
