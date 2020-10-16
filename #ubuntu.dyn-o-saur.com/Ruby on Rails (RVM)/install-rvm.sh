apt-get install build-essential

apt-get install git-core curl libpq-dev

apt-get install mysql-server

mkdir -p ~/.rvm/src/rvm/ && cd ~/.rvm/src && git clone http://github.com/wayneeseguin/rvm.git && cd rvm && ./install

echo '[[ -s "$HOME/.rvm/scripts/rvm" ]] && source "$HOME/.rvm/scripts/rvm"' >> ~/.bashrc

# issue the following command for each user to enable rvm for that user.
source /etc/profile.d/rvm.sh

usermod -a -G rvm raychorn

groups raychorn

apt-get install build-essential bison openssl libreadline6 libreadline6-dev zlib1g zlib1g-dev libssl-dev libyaml-dev libsqlite3-0 libsqlite3-dev sqlite3 libxml2-dev libxslt-dev autoconf libc6-dev ncurses-dev

apt-get install libcurl4-openssl-dev

# switch to a specific user like raychorn

