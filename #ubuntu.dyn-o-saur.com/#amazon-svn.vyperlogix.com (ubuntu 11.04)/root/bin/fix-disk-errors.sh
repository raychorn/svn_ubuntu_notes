#!/bin/bash

apt-get update
apt-get upgrade
apt-get install libgnome2-0 linux-headers-server linux-image-server linux-server linux-tools
touch /forcefsck
reboot now -h
