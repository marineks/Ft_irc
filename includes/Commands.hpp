#ifndef	COMMANDS_HPP
#define COMMANDS_HPP

#include "Irc.hpp"

# define VALID_LEN 17
class Commands
{
	private:
		/* data */
	public:
		std::string	validCmds[VALID_LEN] = {
		"INVITE",
		"JOIN",
		"KICK",
		"KILL",
		"LIST",
		"MDP",
		"MODE",
		"NICK",
		"PART",
		"PING",
		"PONG",
		"PRIVMSG",
		"TOPIC",
		"USER",
		"WHO",
		"WHOIS",
		"WHOWAS"
		};

		Commands(/* args */);
		~Commands();

		int	parseCommand(std::string cmd_line, cmd_struct &cmd_infos);
		// Command list functions

};

#endif