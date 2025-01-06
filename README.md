# SParse
A C to SQLite parser

To compile:
-----------

git clone https://github.com/iamlateforbreakast/SParser.git

cd SParser-master

cd SParser

autoconf

./configure

autoreconf -- install

Debian:
-------
apt install autoconf autotools-dev doxygen
apt install sqlite3 libsqlite3-dev
apt install flex bison

Fedora 41::
----------
sudo dnf install automake doxygen flex bison sqlite sqlite-dev

sudo dnf install git make gcc
