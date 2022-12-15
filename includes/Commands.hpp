#ifndef	COMMANDS_HPP
#define COMMANDS_HPP

#include "Irc.hpp"

class Commands
{
	private:
		/* data */
	public:
		std::string	validCmds[3] = {"MDP",
									"NICK", 
									"USER"};

		Commands(/* args */);
		~Commands();

		int	parseCommand(std::string cmd_line, cmd_struct &cmd_infos);
		// Command list functions

};

#endif