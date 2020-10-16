svn update ~/zoneclient --username read_only --password peekab00

FILES=~/zoneclient/*.sh
chmod +x $FILES
for f in $FILES
do
  dos2unix $f
done
