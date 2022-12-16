#include "Commands.hpp"

Commands::Commands(/* args */)
{
}

Commands::~Commands()
{
}

// exple de ligne : @id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!
// autre exple : /ping localhost devient : PRIVMSG localhost :PING 1671195504 289728
int	Commands::parseCommand(std::string cmd_line, cmd_struct &cmd_infos)
{
	// std::cout << PURPLE << "This is the retrieved cmd_line : " << cmd_line <<  RESET << std::endl;
	
	
	// FIND THE COMMAND, aka the first uppercase word(s) you see in the string
	for (std::string::size_type i = 0; i < cmd_line.size(); i++)
	{
		if (isupper(cmd_line[i]))
			cmd_infos.name.push_back(cmd_line[i]);
		if (islower(cmd_line[i]) && cmd_infos.name.empty() == false)
			break;
	}
	std::cout << RED << "The command name is : " << cmd_infos.name << RESET << std::endl;
	
	// FIND THE PREFIX
	size_t prefix_end = cmd_line.find(cmd_infos.name, 0);
	for (size_t j = 0; j < prefix_end; j++)
		cmd_infos.prefix.push_back(cmd_line[j]);
	std::cout << "Le prefix est : " << YELLOW << cmd_infos.prefix << RESET << std::endl;
	
	// FIND THE MESSAGE
	size_t msg_beginning = cmd_line.find(cmd_infos.name, 0) + cmd_infos.name.length();
	cmd_infos.message = cmd_line.substr(msg_beginning, std::string::npos);
	std::cout << "Le message est : " << GREEN << cmd_infos.message << RESET << std::endl;


	return (SUCCESS);
}