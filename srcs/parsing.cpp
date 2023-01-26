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
	std::string copy = cmd_line;
	if (cmd_line[0] == ':')
		copy.erase(0, copy.find_first_of(' '));
	cmd_infos.name.insert(0, copy, 0, copy.find_first_of(' '));
	
	// PREFIX
	size_t prefix_length = cmd_line.find(cmd_infos.name, 0);
	cmd_infos.prefix.assign(cmd_line, 0, prefix_length);
	
	// MESSAGE
	size_t msg_beginning = cmd_line.find(cmd_infos.name, 0) + cmd_infos.name.length();
	cmd_infos.message = cmd_line.substr(msg_beginning, std::string::npos);
	cmd_infos.message.erase(cmd_infos.message.find("\r"), 1);

	for (size_t i = 0; i < cmd_infos.name.size(); i++)
		cmd_infos.name[i] = std::toupper(cmd_infos.name[i]);
	
	// DEBUG
	// std::cout << "Command : " << RED << cmd_infos.name << RESET << std::endl;
	// std::cout << "Prefix : " << BLUE << cmd_infos.prefix << RESET << std::endl;
	// std::cout << "Message : " << GREEN << cmd_infos.message << RESET << std::endl;
	return (SUCCESS);
}