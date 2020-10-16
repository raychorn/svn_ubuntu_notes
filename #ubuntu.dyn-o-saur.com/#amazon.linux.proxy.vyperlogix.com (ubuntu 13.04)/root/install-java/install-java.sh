#!/bin/bash

tar -xvf jdk-7u17-linux-i586.gz

mkdir -p /usr/lib/jvm/jdk1.7.0

mv jdk1.7.0_17/* /usr/lib/jvm/jdk1.7.0/

update-alternatives --install "/usr/bin/java" "java" "/usr/lib/jvm/jdk1.7.0/bin/java" 1

update-alternatives --install "/usr/bin/javac" "javac" "/usr/lib/jvm/jdk1.7.0/bin/javac" 1 

update-alternatives --install "/usr/bin/javaws" "javaws" "/usr/lib/jvm/jdk1.7.0/bin/javaws" 1

echo "JAVA_HOME=/usr/lib/jvm/jdk1.7.0" >> /etc/profile
echo "PATH=$PATH:$HOME/bin:$JAVA_HOME/bin" >> /etc/profile
echo "export JAVA_HOME" >> /etc/profile
echo "export PATH" >> /etc/profile
