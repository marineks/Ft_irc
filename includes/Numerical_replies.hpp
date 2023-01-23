#ifndef NUMERICAL_REPLIES_HPP
#define NUMERICAL_REPLIES_HPP

void	sendServerRpl(int const client_fd, std::string reply);

# define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")

// INVITE
# define ERR_NEEDMOREPARAMS(client, command) ("461 " + client + " " + command + " :Not enough parameters.\r\n")
# define ERR_NOSUCHCHANNEL(client, channel) ("403 " + client + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(client, channel) ("442 " + client + " " + channel + " :The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(client, nick, channel) ("443 " + client + " " + nick + " " + channel + " :Is already on channel\r\n")
# define RPL_INVITING(client, nick, channel) ("341 " + client + " " + nick + " " + channel + " :Is invited to a channel!\r\n")

// JOIN
# define RPL_JOIN(username, nickname, channel) (":" + nickname + "!" + username + "@localhost JOIN " +  channel + "\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " " + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")

// NAMES
# define RPL_NAMREPLY(client, symbol, channel, list_of_nicks) ("353 " + client + " " + symbol + " #" + channel + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(client, channel) ("366 " + client + " " + channel + " :End of /NAMES list\r\n")

// NICK
# define ERR_NONICKNAMEGIVEN(client) ("431 " + client + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(client, nickname) ("432 " + client + " " + nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nickname) ("433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
# define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")

// OPER
# define ERR_NOOPERHOST(client) ("491 " + client + " :No O-lines for your host\r\n")
# define RPL_YOUREOPER(client) ("381 " + client + " :You are now an IRC operator\r\n")

// PART
# define RPL_PART(user_id, channel, reason) (user_id + " PART #" + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")

// PASS
# define ERR_PASSWDMISMATCH(client) ("464 " + client + " :Password incorrect.\r\n")

// PING
# define RPL_PONG(user_id, token) (user_id + " PONG " + token + "\r\n")

// QUIT
# define RPL_QUIT(user_id, reason) (user_id + " QUIT :Quit: " + reason + "\r\n")
# define RPL_ERROR(user_id, reason) (user_id + " ERROR :" + reason + "\r\n")

// PRIVMSG
# define ERR_NOSUCHNICK(client, target) ("401 " + client + " " + target + " :No such nick/channel\r\n")
# define ERR_NORECIPIENT(client) ("411 " + client + " :No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, message) (":" + nick + "!" + username + "@localhost PRIVMSG" + message + "\r\n")

// TOPIC
# define RPL_TOPIC(client, channel, topic) ("332 " + client + " #" + channel + " " + topic + "\r\n")
# define RPL_NOTOPIC(client, channel) ("331 " + client + " " + channel + ": The topic has been cleared.\r\n")
# define RPL_NEWTOPIC(client, channel, topic) (client + " " + channel + " New topic is " + topic + "\r\n")
# define RPL_DISPLAYTOPIC(client_id, channel) (client_id + " TOPIC " +  channel + "\r\n")

// USER
# define ERR_ALREADYREGISTERED(client) ("462 " + client + " :You may not reregister.\r\n")







#endif