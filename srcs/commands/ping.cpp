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
int	ping(int const client_fd, cmd_struct &cmd)
{
	// checker la string (est-ce que l'identifier existe (registered or online?), est-ce que le channel existe ?)
	std::cout << client_fd << " is the client_fd. Message de la cmd : " << YELLOW << cmd.message << RESET << std::endl;
	
	// renvoyer un PONG avec le même TOKEN
	std::string pong_reply = "PONG" + cmd.message + "\r\n";
	send(client_fd, pong_reply.c_str(), pong_reply.size(), 0);
	
	return (SUCCESS);
}