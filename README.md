# SParse
A C to SQLite parser

To compile:
-----------

```
git clone https://github.com/iamlateforbreakast/SParser.git
cd SParser-main/SParser
autoreconf -vfi
./configure
make
```

Dependencies Debian:
--------------------
```
apt install autoconf autotools-dev doxygen
apt install sqlite3 libsqlite3-dev
apt install flex bison
```

Dependencies Fedora 41:
-----------------------
```
sudo dnf install automake doxygen flex bison sqlite sqlite-devel
sudo dnf install git make gcc
```

