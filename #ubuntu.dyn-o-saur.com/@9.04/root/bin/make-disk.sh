cd /host/ubuntu/disks
dd if=/dev/zero of=$1.disk bs=1MB count=1 seek=100000
mkfs.ext3 -F $1.disk
