#!/bin/bash

time=$(date +%H%M%S)

filename="_"`eval date +%m-%d-%Y`"_"$time

echo $filename

aws ls __vyperlogix_svn_backups__/$filename

