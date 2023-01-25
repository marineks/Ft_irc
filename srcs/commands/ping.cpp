#include "Irc.hpp"
#include "Commands.hpp"

/**
 * @brief The PING command is sent to check the other side of the connection is still
 * 		connected, and/or to check for connection latency.
 * 
 * 		When receiving a PING message, clients or servers must reply to it with a
 * 		PONG message with the same <token> value. 
 * 
 * 		Numeric replies:
 * 		461 - ERR_NEEDMOREPARAMS -  "<client> <command> :Not enough parameters" > DEALT BY IRSSI
 * 		409 - ERR_NOORIGIN - "<client> :No origin specified" (when <token> is empty) > DEALT BY IRSSI
 * 
 * 		IRSSI :
 * 		With this reference client, sending a PING to a server sends a CTCP request
 * 		to a nickname or a channel. Using the char '*' pings every user in a channel.
 * 		Syntax : PING [<nick> | <channel> | *]
 * 
 * @return SUCCESS (0) or FAILURE (1) 
 */
int	ping(Server *server, int const client_fd, cmd_struct &cmd)
{
	Client		&client		= retrieveClient(server, client_fd);
	std::string	nickname	= client.getNickname();
	std::string	username	= client.getUsername();

	if (cmd.message[0] == ' ')
		cmd.message.erase(0, 1);
	cmd.message.insert(0, ":");
	addToClientBuffer(server, client_fd, RPL_PONG(user_id(nickname, username), cmd.message));

	return (SUCCESS);
}