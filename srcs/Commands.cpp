#include "Commands.hpp"

Commands::Commands(/* args */)
{
}

Commands::~Commands()
{
}

int	Commands::parseCommand(std::string cmd_line, cmd_struct &cmd_infos)
{
	std::cout << "This is the retrieved cmd_line : " << cmd_line << std::endl;
	// exple de ligne : @id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!
	
	// trouver le mot en uppercase
	// tu boucles, des que tu trouves des majuscules tu les rentres dans une variable
	// et à l'espace

	// ce qui est devant : prefix


	// ce qui va derriere : message

	return (SUCCESS);
}