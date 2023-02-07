# FT_IRC

<p align="center">  <img src="assets/welcome.png" width="90%">


## About this project


<img src="assets/irc-shema.png">

##### *Proudly coded by [@tmanolis](https://github.com/tmanolis), [@QnYosa](https://github.com/QnYosa) and @marineks :heart:*

## Commands that were implemented

| Command | Description |
| ----------- | ----------- |
| Invite | Title |
| Join | Text |
| Kick | Title |
| Kill | Text |
| List | Title |
| Mode | Text |
| Motd | Title |
| Names | Text |
| Nick | Title |
| Notice | Text |
| Oper | Title |
| Part | Text |
| Pass | Title |
| Ping | Text |
| Privmsg | Text |
| Quit | Title |
| Topic | Text |
| User | Text |

## Some tips, graphs and other ressources

#### Helpful links
- **To understand what exactly is an IRC and begin the project** : [Chirc](http://chi.cs.uchicago.edu/chirc/irc.html) (An irc subject from a Chicago uni) and the next pages too.


- **Regarding the client/server connection** : [Beej's Guide to network programming](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf). This is super super helpful to better understand what is a socket, what the `poll()` function entails, which system calls we should use or in which order, and so on.


<p align="center"> <img src="assets/network.png"> </p>

- **A link to the IRC Client Protocol with a good formatting** : [Modern IRC Client Protocol](https://modern.ircdocs.horse/). You will find there a description of all the commands with correct syntax, expected Numerical Replies,...


#### Tips

- Begin the project by establishing a client/server connection, then build on that : add the signals handling (`CTRL+C`, `CTRL+D`, `CTRL+Z`) and ***only*** then begin the client registration system (`NICK`, `USER`, `PASS`), the server commands (`PING`, `OPER`, `KILL`...) and the channel operations commands (`JOIN`, `PART`, `INVITE`, `KICK`, `PRIVMSG`, `NOTICE`...)


___
- **How to know how to correctly format a RPL to be understood by your IRC client of reference** (for instance, irssi) ?
  
  Join an existing server (try `/connect DALNET`), then enter the command `/RAWLOG OPEN debug.log` [(more doc)](https://irssi.org/documentation/help/rawlog/), and try the commands you're having some troubles with. This will open a log file in your workspace containing the all the exact client/server interactions for these commands, such as :
	```c
	// [...]
	<< JOIN #help // from the client
	>> :nickname!username@host JOIN :#help // reply from the server
	// [...]
	```
  Voilà ! Now you know you forgot the `:` before the channel name! ;)

___
- **Okay, now you have a good format but the code is not clean at all?** Try to use dynamic macros in this way (*we guarantee this is a life changer/saver!*) :
  ```cpp
	#define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
	#define RPL_INVITE(user_id, invited, channel) (user_id + " INVITE " + invited + " #" + channel + "\r\n")
	// [...]
	#define ERR_INVALIDMODEPARAM(client, channel, mode, password) ("696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")
  ```