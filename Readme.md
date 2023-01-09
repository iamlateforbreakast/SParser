To compile:
-----------

gh auth login

gh repo clone iamlateforbreakast/SParse

cd SParse

git status

touch test.txt

git add .

git commit -m "Created test.txt"

git push

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
