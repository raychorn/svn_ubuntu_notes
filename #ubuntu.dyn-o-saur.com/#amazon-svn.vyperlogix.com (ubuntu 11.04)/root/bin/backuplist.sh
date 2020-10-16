#!/bin/bash
export PYTHONPATH=/usr/lib/python2.7/dist-packages

export PASSPHRASE=Peekab00
export AWS_ACCESS_KEY_ID=AKIAI52A6BTLWZHHDLCA
export AWS_SECRET_ACCESS_KEY=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

NAME=$1
if [ ${#NAME} == "0" ]; then
	duplicity list-current-files --encrypt-key=7E59FE27 s3+http://__vyperlogix_svn_backups__
else
	p=$(duplicity list-current-files --encrypt-key=7E59FE27 s3+http://__vyperlogix_svn_backups__ | grep $NAME)
	echo $p
fi

export PASSPHRASE=
export AWS_ACCESS_KEY_ID=
export AWS_SECRET_ACCESS_KEY=