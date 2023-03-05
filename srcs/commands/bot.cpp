#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

static void	botHour(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::string bot)
{
	time_t curr_time;
	curr_time = std::time(NULL);

	tm *tm_local = localtime(&curr_time);
	std::string time = "Current local time: " + std::to_string(tm_local->tm_hour) + ":" + std::to_string(tm_local->tm_min) + ":" + std::to_string(tm_local->tm_sec);
	
	addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), time));
}

void	bot(Server *server, int const client_fd, std::map<const int, Client>::iterator it_client, std::string bot_cmd)
{
   std::cout << "bot cmd : |" << bot_cmd << "|" << std::endl;
   
   std::string bot = "BOT";
   
   // change msg to send into caps
	std::string	validCmds[3] = {
		":HELP",
		":HOUR",
		":RANDOM",
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
		case 1: addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), "Ask me 'HOUR' or 'RANDOM'")); break;
		case 2: botHour(server, client_fd, it_client, bot); break;
		case 3: std::cout << "prout" << std::endl; break;
		default:
			addToClientBuffer(server, client_fd, RPL_PRIVMSG(bot, bot, it_client->second.getNickname(), "Invalid request, ask 'HELP' for more infos"));
	}
}
