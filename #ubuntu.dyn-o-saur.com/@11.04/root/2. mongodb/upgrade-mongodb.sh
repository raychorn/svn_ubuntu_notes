/etc/init.d/mongod stop

wget http://fastdl.mongodb.org/linux/mongodb-linux-i686-2.0.0.tgz
tar -zxf mongodb-linux-i686-2.0.0.tgz -C /usr/local

unlink /usr/local/mongodb
unlink /usr/local/bin/bsondump
unlink /usr/local/bin/mongo
unlink /usr/local/bin/mongod
unlink /usr/local/bin/mongodump
unlink /usr/local/bin/mongoexport
unlink /usr/local/bin/mongofiles
unlink /usr/local/bin/mongoimport
unlink /usr/local/bin/mongorestore
unlink /usr/local/bin/mongos
unlink /usr/local/bin/mongosniff
unlink /usr/local/bin/mongostat

ln -s /usr/local/mongodb-linux-i686-2.0.0 /usr/local/mongodb
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

/etc/init.d/mongod start
