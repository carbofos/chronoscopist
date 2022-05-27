# Chronoscopist
Chronoscopist is the simplest linux time control software to control time and block PC after specified daily time limits.
I did start to develop this project because did not find a good opensource project for my children.


# Description

Chronoscopist is written in C++11 and uses MySQL database to store data and boost::asio library to accept multiple clients.
The project have server and client side binaries.
Server side requires MySQL database.

Now lock and unlock commands are hardcoded in src/ChronoClientConnection.cpp
system("DISPLAY=:0 /usr/bin/xset dpms force off");
system("DISPLAY=:0 /usr/bin/xset dpms force on");

You can change it to the best ones for your client OS. I still did not think regarding the best way to lock/unlock.


# Compile the project

First you should install mysql client libraries with developer headers.
Also boost devel library should be installed.

$ mkdir build
$ cd build
$ cmake ..
$ make

# Mysql setup
$ cp chronoserver.cfg ./build/

Edit chronoserver.cfg and setup mysql prerequisites.
Now all clients have one time limit.
You can change this setting in the same file as timelimit variable ( value should be specified as integer in minutes )


# Server side example:
./chronoscopist-server

Check out once again: chronoserver.cfg should be in the same directory.

# Client side example:
( Change 192.168.1.1 to your chronoscopist server IP )

./chronoscopist-client 192.168.1.1
