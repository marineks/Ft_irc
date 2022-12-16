#include "Commands.hpp"

Commands::Commands(/* args */)
{
}

Commands::~Commands()
{
}

int	Commands::parseCommand(std::string cmd_line, cmd_struct &cmd_infos)
{
	// std::cout << PURPLE << "This is the retrieved cmd_line : " << cmd_line <<  RESET << std::endl;
	// exple de ligne : @id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!
	// autre exple : /ping localhost devient : PRIVMSG localhost :PING 1671195504 289728
	
	// Find the command, aka the first uppercase word(s) you see in the string
	bool cmd_name_found = false;
	for (std::string::size_type i = 0; i < cmd_line.size(); i++)
	{
		if (isupper(cmd_line[i]) && cmd_name_found == false)
			cmd_infos.name.push_back(cmd_line[i]);
		if (islower(cmd_line[i]) && cmd_infos.name.empty() == false)
			cmd_name_found = true;
	}
	// std::cout << RED << "The command name is : " << cmd_infos.name << RESET << std::endl;
	

	// ce qui est devant : prefix


	// ce qui va derriere : message

	return (SUCCESS);
}