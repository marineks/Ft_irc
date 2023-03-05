#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

void	bot(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::string bot_cmd)
{
   std::cout << "bot cmd : |" << bot_cmd << "|" << std::endl;
   
   std::string bot = "BOT";
   // change msg to send into caps
	std::string	validCmds[3] = {
		"HELP",
		"JOIN",
		"KICK",
		};

   int index = 0;
	while (index < 3)
	{
		if (bot_cmd == validCmds[index])
			break;
		index++;
	}
	switch (index + 1)
	{
		case 1: std::cout << "prout" << std::endl;
		case 2: std::cout << "prout" << std::endl;
		case 3: std::cout << "prout" << std::endl;
		default:
			addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), "Invalid request, ask 'HELP' for more infos"));
	}
}