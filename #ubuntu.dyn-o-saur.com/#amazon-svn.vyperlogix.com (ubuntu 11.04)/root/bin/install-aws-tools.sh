#!/bin/bash

if [ -f aws ]; then
	echo "aws already exists"
else
	wget https://raw.github.com/timkay/aws/master/aws
fi

perl aws --install
