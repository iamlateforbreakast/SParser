# SParse
A C to SQLite parser

To compile:
-----------

git clone https://github.com/iamlateforbreakast/SParse.git

cd SParse

git status

git branch update_test_file

git checkout update_test_file

touch test.txt

git add .

git commit -m "Created test.txt"

git push --set-upstream origin update_test_file

git checkout master

git pull origin master

git merge update_test_file

git push origin master

autoreconf -- install
--------------------------------
git fetch --all

git reset --hard origin/master

git push origin master


gh auth login --with-token < token.txt

Debian:
-------
apt install autoconf autotools-dev doxygen
apt install sqlite3 libsqlite3-dev
apt install flex bison

Fedora 35::
----------
sudo dnf install autoconf automake doxygen
