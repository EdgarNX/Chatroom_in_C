# Build
Run make in the repository
* `make` to install program
* `make clean` to clean from object files
* `make uninstall` to uninstall program

# Using
`./server [port]` to run server

# Chatroom for 100 users with few commands and authorization system.
* Sending messages;
* Sending private messages;
* Signing up & signing in;
* Changing nickname & password;

# Commands list
<b>\login [nickname] [password]</b> - sign in<br>
<b>\register [nickname] [password]</b> - sign up<br>
<b>\name [nickname]</b> - change nickname<br>
<b>\passwd [old] [new]</b> - change password<br>
<b>\pm [user] [message]</b> - send private message<br>
<b>\online</b> - print online users list<br>
<b>\h \help</b> - print available commands list<br>
<b>\q \quit \exit</b> - leave server<br>
