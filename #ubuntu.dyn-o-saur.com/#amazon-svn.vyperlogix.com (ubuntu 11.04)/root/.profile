# ~/.profile: executed by Bourne-compatible login shells.

if [ "$BASH" ]; then
  if [ -f ~/.bashrc ]; then
    . ~/.bashrc
  fi
fi

if [ -d "$HOME/bin" ] ; then
    PATH="$HOME/bin:$PATH"
fi

mesg n

export PYTHONPATH=$HOME/python/libs/vyperlogix_2_5_5.zip:/usr/share/pyshared

#$HOME/lib/python2.5/site-packages:/usr/share/python-support/python-webpy:/usr/share/python-support/python-openssl:/usr/share/python-support/python-mysqldb:/usr/share/python-support/python-gobject:/usr/share/python-support/python-gnupginterface:/usr/share/python-support/python-flup:/usr/share/python-support/python-dbus:/usr/share/python-support/python-cheetah

