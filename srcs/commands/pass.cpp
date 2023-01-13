#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"


std::string	retrievePassword(std::string msg_to_parse);

/**
 * @brief The PASS command is used to set a ‘connection password’.
 * The password supplied must match the one defined in the server configuration.
 * 	
 * Syntax: PASS <password>
 * 
 * Numeric replies:
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_ALREADYREGISTERED (462)
 * 	ERR_PASSWDMISMATCH (464)
 * 
 * Example :
 *  [CLIENT] /PASS secretpassword
 */
int		pass(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	Client		client		= retrieveClient(server, client_fd);
	std::string	password	= retrievePassword(cmd_infos.message);

	if (server->getPassword() == password)
	{
		return (SUCCESS);
	}
	else
	{
		sendServerRpl(client_fd, ERR_PASSWDMISMATCH(client.getNickname()));
		return (FAILURE);
	}
		
}

std::string	retrievePassword(std::string msg_to_parse)
{
	std::string	password;

	size_t i = 0;
	while (msg_to_parse[i] == ' ')
		i++;
	while (msg_to_parse[i] && msg_to_parse[i] != ' ')
		password += msg_to_parse[i++];
	return (password);
}
