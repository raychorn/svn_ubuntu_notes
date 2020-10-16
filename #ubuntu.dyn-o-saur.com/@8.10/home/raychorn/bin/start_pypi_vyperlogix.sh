export PYTHONPATH=/home/raychorn/lib/python2.5/site-packages:/usr/share/python-support/python-webpy:/usr/share/python-support/python-openssl:/usr/share/python-support/python-mysqldb:/usr/share/python-support/python-gobject:/usr/share/python-support/python-gnupginterface:/usr/share/python-support/python-flup:/usr/share/python-support/python-dbus:/usr/share/python-support/python-cheetah

export PYTHONPATH=/home/raychorn:/home/raychorn/pypi_info:/home/raychorn/lib/python2.5/site-packages:/home/raychorn/python25/lib/VyperLogixLib-1.0-py2.5.egg:/home/raychorn/python25/lib/VyperLogixPyaxLib-1.0-py2.5.egg:/home/raychorn/python25/lib/SQLAlchemy-0.5.3-py2.5.egg:/home/raychorn/python25/lib/Django-0.96.3-py2.5.egg:/usr/lib/python2.5/site-packages:/var/lib/python-support/python2.5:$PYTHONPATH

python /home/raychorn/pypi_info/manage.pyc runfcgi method=threaded host=127.0.0.1 port=9300 protocol=scgi pidfile=/home/raychorn/pypi_info.pid
