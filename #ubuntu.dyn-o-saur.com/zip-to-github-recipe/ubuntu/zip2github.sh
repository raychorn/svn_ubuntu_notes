cd ~
mkdir @1

sudo apt-get install zip
sudo apt-get install unzip

cd @1

#upload a zip file

#make github repo

# https://help.github.com/articles/generating-ssh-keys

# chmod 400 ~/.ssh/id_rsa

unzip MyRailsApp.zip
cd MyRailsApp
git init
git add *
git commit -m "first commit"
git remote add origin git@github.com:raychorn/MyRailsApp.git
git push -u origin master
