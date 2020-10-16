#!/bin/bash
export PYTHONPATH=/usr/lib/python2.7/dist-packages

export PASSPHRASE=Peekab00
export AWS_ACCESS_KEY_ID=AKIAI52A6BTLWZHHDLCA
export AWS_SECRET_ACCESS_KEY=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

FILE=$1

if [ -d /root/#svn_backups_restore ]; then
	echo "/root/#svn_backups_restore already exists"
else
	mkdir /root/#svn_backups_restore
fi

if [ ${#FILE} == "0" ]; then
	./backup-list.sh
else
	duplicity --file-to-restore $FILE s3+http://__vyperlogix_svn_backups__ /root/#svn_backups_restore/$FILE --encrypt-key=7E59FE27 --sign-key=7E59FE27 -vinfo
fi

export PASSPHRASE=
export AWS_ACCESS_KEY_ID=
export AWS_SECRET_ACCESS_KEY=