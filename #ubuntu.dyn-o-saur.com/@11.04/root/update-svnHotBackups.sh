svn update ~/svnHotBackups --username read_only --password peekab00

FILES=~/svnHotBackups/*.sh
chmod +x $FILES
for f in $FILES
do
  dos2unix $f
done
