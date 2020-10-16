#!/bin/sh
#
#   +----------------------------------------------------------------------+
#   | Zend installation script                                             |
#   +----------------------------------------------------------------------+
#   | Copyright (c) 1998-2005 Zend Technologies Ltd.                       |
#   +----------------------------------------------------------------------+
#   | The contents of this source file is the sole property of             |
#   | Zend Technologies Ltd.  Unauthorized duplication or access is        |
#   | prohibited.                                                          |
#   +----------------------------------------------------------------------+
#   | Authors: Michael Spector <michael@zend.com>                          |
#   |          Anya Tarnyavsky <anya@zend.com>                             |
#   +----------------------------------------------------------------------+
#

INSTALL_DIR=./zui_files
PHP_SCRIPT=install.php
ZEND_TMPDIR=/tmp/zend_install.$$
ORIGINAL_CWD=`pwd`
CALLING_SCRIPT=$0

error ()
{
	echo "ERROR: "$1
	exit
}

freebsd_libc_patch()
{
	if [ ! -e /usr/lib/libc.so.3 ]; then
		if [ -e /lib/libc.so.5 ]; then
			ln -s /lib/libc.so.5 /usr/lib/libc.so.3
		elif [ -e /usr/lib/libc.so.5 ]; then
			ln -s /usr/lib/libc.so.5 /usr/lib/libc.so.3
		elif [ -e /usr/lib/libc.so.4 ]; then
			ln -s /usr/lib/libc.so.4 /usr/lib/libc.so.3
		elif [ -e /usr/lib/libc.so ]; then
			ln -s /usr/lib/libc.so /usr/lib/libc.so.3
		fi
	fi
		
	if [ ! -e /lib/libm.so.2 ]; then
		if [ -e /lib/libm.so.3 ]; then
			ln -s /lib/libm.so.3 /lib/libm.so.2
		elif [ -e /lib/libm.so ]; then
			ln -s /lib/libm.so /lib/libm.so.2
		fi
	fi
}

darwin_libdl_patch()
{
	if [ -f libdl.dylib ]; then
		if [ ! -d /usr/local/lib ]; then
			mkdir -p /usr/local/lib 2> /dev/null || error "Cannot mkdir: /usr/local/lib"
		fi
		if [ ! -e /usr/local/lib/libdl.dylib ]; then
			cp -f libdl.dylib /usr/local/lib/libdl.dylib
		fi
	fi
}

create_php_ini()
{
	if [ ! -f php-install.ini ]; then
		touch php-install.ini
		if [ -f ../data/ZendOptimizer.so ]; then
			echo 'zend_extension=../data/ZendOptimizer.so' >> php-install.ini
		fi
		echo 'memory_limit=100M' >> php-install.ini
	fi
}

check_root_permissions()
{
	ID="id -u"
	MYUID=`$ID 2> /dev/null`

	if [ -z "$MYUID" ]; then
		MYUID=`/usr/xpg4/bin/$ID 2> /dev/null`;
	fi

	if [ ! -z "$MYUID" ]; then
		if [ $MYUID != 0 ]; then
			error "You need root privileges to run this script!";
		fi
	fi
}

cleanup ()
{
	if [ ! -z "$ZEND_TMPDIR" ] && [ -d "$ZEND_TMPDIR" ];
	then
		for saved_config in `ls $ZEND_TMPDIR/saved/_* 2> /dev/null`;
		do
			orig_config=`basename $saved_config | sed 's/_/\//g'`
			nbytes_orig=`wc -c $orig_config | sed 's/^ *//' | cut -d' ' -f1`
			nbytes_saved=`wc -c $saved_config | sed 's/^ *//' | cut -d' ' -f1`

			# If original config file was changed, restore it:
			if [ ! -s $orig_config -o $nbytes_orig -lt $nbytes_saved ]; then
				rm -f $orig_config
				mv $saved_config $orig_config
			fi
		done
		rm -rf $ZEND_TMPDIR
	fi

	stty echo
	clear
	echo
	echo "Installation script was aborted. The process was NOT completed successfully."
	echo
	exit 1
}

check_root_permissions

# Go to the package directory:
if [ ! -d $INSTALL_DIR ]; then
	cd `dirname $0` 2> /dev/null
fi

cd $INSTALL_DIR 2> /dev/null || error "Cannot CD to install directory: "$INSTALL_DIR

if [ ! -f ./php -o ! -x ./php ]; then
	error "Executable file: ./php doesn't exist in "$INSTALL_DIR
fi

[ `uname` = "FreeBSD" ] && freebsd_libc_patch
[ `uname` = "Darwin" ] && darwin_libdl_patch

create_php_ini

trap 'cleanup' 2 15

# execute the main PHP script
ZEND_TMPDIR=$ZEND_TMPDIR CALLING_SCRIPT=$CALLING_SCRIPT ORIGINAL_CWD=$ORIGINAL_CWD \
	./php -c ./php-install.ini -q $PHP_SCRIPT $@

