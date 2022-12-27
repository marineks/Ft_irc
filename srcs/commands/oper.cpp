#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"

/**
 * @brief The OPER command is used by a normal user to obtain IRC operator privileges. 
 * 	Both parameters are required for the command to be successful.
 * 
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
void	oper(Server server, cmd_struct cmd_infos)
{
	// TODO: coder le parsing du command.message pour arriver à operator name, channelName et clientName
	std::string channelName;
	std::string operatorName;
	std::string password;


	if (password != _operatorPassword) // NOTE: devrait etre une fonction membre d'un channel, pas du server
	{
		std::cout << "Wrong Password\n";
		return ;
	}

	std::map<std::string, Channel>			 channels = server.getChannels();
	std::map<std::string, Channel>::iterator it;
	it = channels.find(channelName);
	if (it == channels.end())
	{
		std::cout << "That channel doesn't exist\n";
		return ;
	}
	if (it->second.isOperator(operatorName) == false)
		it->second.addFirstOperator(operatorName); // NOTE: refacto cette fonction qui fait comme AddOperator
}

void	Server::printOper(std::string &channelName)
{
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
	if (it == _channels.end())
	{
		std::cout << "That channel doesn't exit\n";
		return ;
	}
	it->second.printOperators();
}