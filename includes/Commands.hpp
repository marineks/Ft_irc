#ifndef	COMMANDS_HPP
#define COMMANDS_HPP

#include "Irc.hpp"

# define VALID_LEN 18
class Commands
{
	private:
		/* data */
	public:
		std::string	validCmds[VALID_LEN] = {
		"CAP LS",
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