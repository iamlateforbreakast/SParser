# SParse
A C to SQLite parser

To compile:
-----------

git clone iamlateforbreakast/SParse

cd SParse

git status

git branch update_test_file

git checkout update_test_file

touch test.txt

git add .

git commit -m "Created test.txt"

git push --set-upstream origin update_test_file

autoreconf -- install
--------------------------------
git fetch --all

git reset --hard origin/master

git push origin master

git clone https://github.com/iamlateforbreakast/SParse.git

gh auth login --with-token < token.txt

Debian:
-------
Apt install autotools-dev
Apt install autoconf
Apt install sqlite3 libsqlite3-dev
Apt install flex bison
Apt install doxygen

Fedora 35::
----------
sudo dnf install autoconf automake doxygen
