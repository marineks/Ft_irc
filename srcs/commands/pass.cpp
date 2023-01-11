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
void	pass(Server *server, int const client_fd, cmd_struct cmd_infos)
{
	std::cout << "The Server password is : " << server->getPassword() << std::endl;

	std::cout << "Le cmd.msg est: " << cmd_infos.message << std::endl;
	
	std::string password = retrievePassword(cmd_infos.message); // parse le password
	std::cout << "Le password fourni par le client est : " << password << std::endl;

	if (server->getPassword() == password)
	{
		std::cout << "Yeah" << std::endl;
		// comment faire une connexion ? 
		// envoyer un msg au client ?
		std::string test = "Password is accepted.\r\n ";
		sendServerRpl(client_fd, test);
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
	std::cout << "Le password est : " << password << std::endl;
	return (password);
}
