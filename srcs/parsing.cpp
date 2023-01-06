#include "Irc.hpp"
#include "Commands.hpp"

/**
 * @brief 
 * 
 * 	exple de ligne : @id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!
 *  autre exple : /ping localhost devient : PRIVMSG localhost :PING 1671195504 289728
 * 
 */
int	parseCommand(std::string cmd_line, cmd_struct &cmd_infos)
{
	// COMMAND
	for (std::string::size_type i = 0; i < cmd_line.size(); i++)
	{
		if (isupper(cmd_line[i]))
			cmd_infos.name.push_back(cmd_line[i]);
		if (islower(cmd_line[i]) && cmd_infos.name.empty() == false)
			break;
	}
	
	// PREFIX
	size_t prefix_length = cmd_line.find(cmd_infos.name, 0);
	cmd_infos.prefix.assign(cmd_line, 0, prefix_length);
	
	// MESSAGE
	size_t msg_beginning = cmd_line.find(cmd_infos.name, 0) + cmd_infos.name.length();
	cmd_infos.message = cmd_line.substr(msg_beginning, std::string::npos);

	// DEBUG
	std::cout << "Command : " << RED << cmd_infos.name << RESET << std::endl;
	std::cout << "Prefix : " << BLUE << cmd_infos.prefix << RESET << std::endl;
	std::cout << "Message : " << GREEN << cmd_infos.message << RESET << std::endl;

	return (SUCCESS);
}