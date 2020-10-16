wget http://fastdl.mongodb.org/linux/mongodb-linux-i686-1.8.3.tgz
tar -zxf mongodb-linux-i686-1.8.3.tgz -C /usr/local

ln -s /usr/local/mongodb-linux-i686-1.8.3 /usr/local/mongodb
ln -s /usr/local/mongodb/bin/bsondump /usr/local/bin/bsondump
ln -s /usr/local/mongodb/bin/mongo /usr/local/bin/mongo
ln -s /usr/local/mongodb/bin/mongod /usr/local/bin/mongod
ln -s /usr/local/mongodb/bin/mongodump /usr/local/bin/mongodump
ln -s /usr/local/mongodb/bin/mongoexport /usr/local/bin/mongoexport
ln -s /usr/local/mongodb/bin/mongofiles /usr/local/bin/mongofiles
ln -s /usr/local/mongodb/bin/mongoimport /usr/local/bin/mongoimport
ln -s /usr/local/mongodb/bin/mongorestore /usr/local/bin/mongorestore
ln -s /usr/local/mongodb/bin/mongos /usr/local/bin/mongos
ln -s /usr/local/mongodb/bin/mongosniff /usr/local/bin/mongosniff
ln -s /usr/local/mongodb/bin/mongostat /usr/local/bin/mongostat

wget https://github.com/ijonas/dotfiles/raw/master/etc/init.d/mongod
mv mongod /etc/init.d/mongod
chmod +x /etc/init.d/mongod

useradd mongodb
mkdir -p /var/lib/mongodb
mkdir -p /var/log/mongodb
chown mongodb:mongodb /var/lib/mongodb
chown mongodb:mongodb /var/log/mongodb

update-rc.d mongod defaults

/etc/init.d/mongod start
