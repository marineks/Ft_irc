#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

/**
 * @brief The OPER command is used by a normal user to obtain IRC operator privileges.
 * 	Both parameters are required for the command to be successful.
 *
 *  Command: OPER
 *  Parameters: <name> <password>
 *
 * Numeric Replies:
 *
 * ERR_NEEDMOREPARAMS (461)
 * ERR_PASSWDMISMATCH (464)
 * ERR_NOOPERHOST (491)
 * RPL_YOUREOPER (381)
 *
 * Example:
 *  [CLIENT] OPER foo bar
 *  [SERVER] ; Attempt to register as an operator using a name of "foo" and the password "bar".
 */
void oper(Server *server, int const client_fd, cmd_struct cmd_infos)
{

	(void) server;
	(void) client_fd;
	(void) cmd_infos;

}

// 
// 	std::string operatorName;
// 	std::string password;

// 	std::map<std::string, Channel> channels = server.getChannels();
// 	std::map<std::string, Channel>::iterator it;
// 	it = channels.find(channelName);

// 	if (password != it.getOperatorPassword()) 
// 	{
// 		std::cout << "Wrong Password\n";
// 		return;
// 	}
// 	if (it == channels.end())
// 	{
// 		std::cout << "That channel doesn't exist\n";
// 		return ;
// 	}
// 	if (it->second.isOperator(operatorName) == false)
// 		it->second.addFirstOperator(operatorName); // NOTE: refacto cette fonction qui fait comme AddOperator