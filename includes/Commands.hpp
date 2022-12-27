#ifndef	COMMANDS_HPP
#define COMMANDS_HPP

#include "Irc.hpp"
#include "Client.hpp"

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

		int		parseCommand(std::string cmd_line, cmd_struct &cmd_infos);
		// Command list functions
		int		ping(int const client_fd, cmd_struct &cmd);
		void	join(Client &client, std::string &channelName);
		void	quit(std::string &channelName, std::string &clientName);
		void	kick(std::string &operatorName, std::string &channelName, std::string &clientName);
		void	ban(std::string &operatorName, std::string &channelName, std::string &banned_ame);
		void	unban(std::string &operatorName, std::string &channelName, std::string &unbanned_name);
		void	oper(std::string channelName, std::string operatorName, std::string password);
};

#endif