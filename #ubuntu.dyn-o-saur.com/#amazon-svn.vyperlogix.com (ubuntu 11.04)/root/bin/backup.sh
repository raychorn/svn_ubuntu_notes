#!/bin/bash
export PYTHONPATH=/usr/lib/python2.7/dist-packages

export PASSPHRASE=Peekab00
export AWS_ACCESS_KEY_ID=AKIAI52A6BTLWZHHDLCA
export AWS_SECRET_ACCESS_KEY=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

time=$(date +%H%M%S)
folder="_"`eval date +%m-%d-%Y`"_"$time"_"

NAME="repo1-14666.tar.gz"

aws mkdir __vyperlogix_svn_backups__/$folder

pid=$(ps aux | grep duplicity | grep -v grep | awk '{print $2}' | tail -n 1)

if [ ${#pid} == "0" ]; then
	duplicity remove-older-than 1M --encrypt-key=7E59FE27 --sign-key=7E59FE27 s3+http://__vyperlogix_svn_backups__/$folder
 
	duplicity --full-if-older-than 14D --allow-source-mismatch --encrypt-key=7E59FE27 --sign-key=7E59FE27 /var/local/svn/#svn_backups/repo1 s3+http://__vyperlogix_svn_backups__/$folder
 	
	p=$(duplicity list-current-files --encrypt-key=7E59FE27 s3+http://__vyperlogix_svn_backups__/$folder | grep $NAME)
	if [ $p == $NAME ]; then
		echo "${NAME} is present..."
	else
		echo "${NAME} is not present..."
	fi
else
	echo "Running svnHotBackups.py - Cannot run more than one instance at a time !!!"
fi
 
export PASSPHRASE=
export AWS_ACCESS_KEY_ID=
export AWS_SECRET_ACCESS_KEY=