#ifndef NUMERICAL_REPLIES_HPP
#define NUMERICAL_REPLIES_HPP

void	sendServerRpl(int const client_fd, std::string reply);

// INVITE
# define ERR_NEEDMOREPARAMS(client, command) ("461 " + client + " " + command + " :Not enough parameters\r\n")
# define ERR_NOSUCHCHANNEL(client, channel) ("403 " + client + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(client, channel) ("442 " + client + " " + channel + " :You're not on that channel\r\n")
# define ERR_USERONCHANNEL(client, nick, channel) ("443 " + client + " " + nick + " " + channel + " :Is already on channel\r\n")
# define RPL_INVITING(client, nick, channel) ("341 " + client + " " + nick + " " + channel + " :Is invited to a channel!\r\n")

// NICK
# define ERR_NONICKNAMEGIVEN(client) ("431 " + client + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(client, nick) ("432 " + client + " " + nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nick) ("433 " + client + " " + nick + " :Nickname is already in use.\r\n")
# define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost :" + oclient + " changed their nickname to " + client + "\r\n")













#endif