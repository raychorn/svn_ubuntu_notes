#!/bin/bash

wget "http://s3.amazonaws.com/rds-downloads/RDSCli.zip"
unzip RDSCli.zip

mkdir /opt/RDSCli-1.13.002

mv RDSCli-1.13.002/* /opt/RDSCli-1.13.002

export AWS_RDS_HOME="/opt/RDSCli-1.13.002" >> /etc/profile
echo "PATH=$PATH:$AWS_RDS_HOME/bin" >> /etc/profile

