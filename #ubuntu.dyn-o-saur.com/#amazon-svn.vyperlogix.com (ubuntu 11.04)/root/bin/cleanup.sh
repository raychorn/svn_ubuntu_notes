#!/bin/bash

src="/root/#svn_backups/repo1/_10-13-2011_012809_"
 
IFS=$'\n'
 
for dir in `ls "$src/"`
do
  if [ -f "$src/$dir" ]; then
    rm -f -R $src/$dir
  fi
done

rmdir $src
