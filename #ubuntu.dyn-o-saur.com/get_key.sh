KEY=$1
gpg --keyserver subkeys.pgp.net --recv $KEY
gpg --export --armor $KEY > $KEY.key
apt-key add $KEY.key