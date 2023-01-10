#ifndef NUMERICAL_REPLIES_HPP
#define NUMERICAL_REPLIES_HPP

void	sendServerRpl(int const client_fd, std::string reply);

// # define str(param) std::string(param)

// INVITE
# define ERR_NEEDMOREPARAMS(client, command) ("461 " + client + " " + command + " :Not enough parameters\r\n")
# define ERR_NOSUCHCHANNEL(client, channel) ("403 " + client + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(client, channel) ("442 " + client + " " + channel + " :You're not on that channel\r\n")
# define ERR_USERONCHANNEL(client, nick, channel) ("443 " + client + " " + nick + " " + channel + " :Is already on channel\r\n")
# define RPL_INVITING(client, nick, channel) ("341 " + client + " " + nick + " " + channel + " :Is invited to a channel!\r\n")
#endif